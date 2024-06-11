#include "xray/perf_xray.hxx"

void PerfXray::update() {
    static bool vsync = true;

    if (ImGui::Begin("Performance")) {

        for (const auto& t : Profiler::timers()) {
            const auto text = fmt::format("{}: {} µs", t.first, t.second.duration());
            ImGui::Text("%s", text.c_str());
        }

        ImGui::Text("FPS: %i", Profiler::get_fps());

        if (ImGui::Checkbox("VSync", &vsync)) {
            SDL_GL_SetSwapInterval(vsync ? 1 : 0);
        }
    }
    ImGui::End();
}
