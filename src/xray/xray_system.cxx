#include "xray_system.hxx"

void j_xray_system::on_load() {
    window_ = &game->env().window();

    imgui_context_ = ImGui::CreateContext();
    if (!imgui_context_) {
        LOG(ERROR) << "Could not create imgui context";
        return;
    }
    auto gfx { game->systems()->get<j_gfx_system>() };

    dispatcher_->sink<j_post_render_event>().connect<&j_xray_system::immediate_on_post_render>(this);

    ImGui_ImplSDL2_InitForOpenGL(game->env().window(), gfx->gl_context());
    ImGui_ImplOpenGL3_Init();

    xrays_.emplace_back(std::make_unique<j_script_xray>());
}

void j_xray_system::on_unload() {
    if (imgui_context_) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext(imgui_context_);
    }
}

void j_xray_system::update(uint32_t delta_time) {
    SDL_Event e;

    while (SDL_PeepEvents(&e, 1, SDL_GETEVENT, SDL_EventType::SDL_KEYDOWN, SDL_EventType::SDL_MOUSEWHEEL)) {
        ImGui_ImplSDL2_ProcessEvent(&e);
    }
}

void j_xray_system::immediate_on_post_render(const j_post_render_event&) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window_->handle());
    ImGui::NewFrame();

    for (const auto& xray : xrays_) {
        xray->update();
    }

    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
