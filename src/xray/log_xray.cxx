#include "log_xray.hxx"

void LogXray::update() {
    struct Callbacks {
        static i32 on_search(ImGuiInputTextCallbackData* data) {
            auto filter { static_cast<LogFilter*> (data->UserData) };
            filter->results.clear();
            filter->active = data->BufTextLen > 0;
            if (!filter->active) {
                return 0;
            }
            const std::string_view needle (
                data->Buf,
                static_cast<std::size_t> (data->BufTextLen)
            );

            for (const auto& entry : Log::logs_) {
                const auto it = std::search(
                    entry.message.cbegin(), entry.message.cend(),
                    needle.cbegin(), needle.cend(),
                    [](const char a, const char b) -> bool {
                        return std::toupper(a) == std::toupper(b);
                    }
                );
                if (it != entry.message.end()) {
                    filter->results.push_back(entry);
                }
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
    const auto& store { filter_.active ? filter_.results : Log::logs_ };
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
                    case LogEntry::LogLevel::Info:
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                        break;
                    case LogEntry::LogLevel::Warn:
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.815f, 0.0f, 1.0f));
                        break;
                    case LogEntry::LogLevel::Error:
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.3f, 0.3f, 1.0f));
                        break;
                    default:
                        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
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
