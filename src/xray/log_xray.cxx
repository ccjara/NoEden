#include "log_xray.hxx"

void LogXray::update() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(2.0f, 2.0f));
    ImGui::Begin("Log");

    constexpr auto table_flags {
        ImGuiTableFlags_BordersInnerV |
        ImGuiTableFlags_SizingFixedFit |
        ImGuiTableFlags_RowBg |
        ImGuiTableFlags_ScrollY
    };
    static bool follow { true };
    const bool follow_pressed { ImGui::Checkbox("Follow", &follow) };
    const auto& logs { Log::logs_ };
    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(2.0f, 5.0f));
    if (ImGui::BeginTable("log_table", 2, table_flags)) {
        ImGui::TableSetupScrollFreeze(0, 1); // keep header in view
        ImGui::TableSetupColumn("Time");
        ImGui::TableSetupColumn("Message");
        ImGui::TableHeadersRow();

        ImGuiListClipper clipper;
        clipper.Begin(logs.size());

        while (clipper.Step()) {
            for (i32 row = clipper.DisplayStart; row < clipper.DisplayEnd; ++row) {
                const auto& entry { logs[row] };

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
