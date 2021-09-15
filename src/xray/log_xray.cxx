#include "log_xray.hxx"

void LogXray::update() {
    struct Callbacks {
        static i32 on_search(ImGuiInputTextCallbackData* data) {
            auto filter { static_cast<LogFilter*> (data->UserData) };
            filter->results.clear();
            filter->message_contains = std::string_view(
                data->Buf,
                static_cast<std::size_t> (data->BufTextLen)
            );
            if (!filter->message_contains.length()) {
                return 0;
            }
            for (const auto& entry : Log::logs_) {
                const auto it = std::search(
                    entry.message.cbegin(),
                    entry.message.cend(),
                    filter->message_contains.cbegin(),
                    filter->message_contains.cend(),
                    [](const char a, const char b) -> bool {
                        return std::toupper(a) == std::toupper(b);
                    }
                );
                if (it == entry.message.end()) {
                    continue;
                }
                filter->results.push_back(entry);
            }
            return 0;
        }
    };

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
    ImGui::Begin("Log");

    constexpr auto max_search_length { 255U };
    constexpr auto table_flags {
        ImGuiTableFlags_BordersInnerV |
        ImGuiTableFlags_SizingFixedFit |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_ScrollY
    };
    static bool follow { true };
    static char search_buffer[max_search_length];
    static u16 log_capacity = static_cast<u16> (Log::max_entries_);

    const auto region_avail { ImGui::GetContentRegionAvail() };
    ImGui::ShowDemoWindow();
    ImGui::SetNextItemWidth(region_avail.x * 0.3f);
    ImGui::InputText(
        "Filter",
        search_buffer,
        max_search_length,
        ImGuiInputTextFlags_CallbackEdit,
        Callbacks::on_search,
        static_cast<void *> (&filter_)
    );
    ImGui::SameLine();
    const bool follow_pressed { ImGui::Checkbox("Follow", &follow) };
    ImGui::SameLine();
    ImGui::SetNextItemWidth(ImGui::GetFontSize() * 4.0f);
    if (ImGui::InputScalar("Capacity", ImGuiDataType_U16, (void*) &log_capacity)) {
        log_capacity = std::max<u16>(log_capacity, 1);
        Log::set_capacity(log_capacity);
    }
    ImGui::SameLine();
    if (ImGui::Button("Clear")) {
        Log::logs_.clear();
    }

    struct LevelFilter {
        LogLevel level;
        const ImVec4& color;
        const char* label;
    };

    constexpr LevelFilter level_filter[] = {
        { LogLevel::Debug, COLOR_DEBUG, "D" },
        { LogLevel::Info, COLOR_INFO, "I" },
        { LogLevel::Warn, COLOR_WARN, "W" },
        { LogLevel::Error, COLOR_ERROR, "E" }
    };

    for (auto& entry : level_filter) {
        const bool active { Log::level_ == entry.level };
        constexpr float button_width { 20.0f };
        const float v = 0.1f + static_cast<float>(active) * 0.1f;
        ImGui::SameLine(0, 0);
        if (entry.level == LogLevel::Debug) {
            ImGui::SetCursorPosX(
                ImGui::GetCursorPosX()
                + ImGui::GetContentRegionAvail().x
                - (button_width) * 4
            );
        }
        ImGui::PushStyleColor(ImGuiCol_Text, entry.color);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(v, v, v, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));

        if (active) {
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
        }
        if (ImGui::Button(entry.label, ImVec2(button_width, 0.0f))) {
            Log::set_level(entry.level);
        }
        if (active) {
            ImGui::PopStyleVar();
            ImGui::PopStyleColor();
        }
        ImGui::PopStyleColor(4);
    }

    const auto& store = filter_.message_contains.length() > 0
        ? filter_.results
        : Log::logs_;

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(2.0f, 5.0f));
    if (ImGui::BeginTable("log_table", 2, table_flags)) {
        ImGui::TableSetupScrollFreeze(0, 1); // keep header in view
        ImGui::TableSetupColumn("Time");
        ImGui::TableSetupColumn("Message");
        ImGui::TableHeadersRow();

        ImGuiListClipper clipper;

        clipper.Begin(store.size());

        while (clipper.Step()) {
            for (i32 row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) {
                const auto& entry { store[row] };

                ImGui::TableNextRow();
                ImGui::TableNextColumn();
                ImGui::TextUnformatted(entry.time_point_formatted.c_str());
                ImGui::TableNextColumn();
                switch (entry.level) {
                    case LogLevel::Info:
                        ImGui::PushStyleColor(ImGuiCol_Text, COLOR_INFO);
                        break;
                    case LogLevel::Warn:
                        ImGui::PushStyleColor(ImGuiCol_Text, COLOR_WARN);
                        break;
                    case LogLevel::Error:
                        ImGui::PushStyleColor(ImGuiCol_Text, COLOR_ERROR);
                        break;
                    default:
                        ImGui::PushStyleColor(ImGuiCol_Text, COLOR_DEBUG);
                        break;
                }
                ImGui::TextUnformatted(entry.message.c_str());
                ImGui::PopStyleColor();
            }
        }

        const bool is_at_bottom { ImGui::GetScrollY() >= ImGui::GetScrollMaxY() };

        if (!is_at_bottom && !follow_pressed) {
            follow = false;
        }

        if (follow && is_at_bottom || follow_pressed) {
            ImGui::SetScrollHereY(1.0f);
        }

        ImGui::EndTable();
    }
    ImGui::PopStyleVar(); // cell padding
    ImGui::End();
    ImGui::PopStyleVar(); // window padding
}
