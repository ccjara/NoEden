#include "xray.hxx"

Xray::Xray(Window& window, entt::dispatcher& dispatcher) :
    window_ { window },
    dispatcher_ { dispatcher } {
}

void Xray::startup(SDL_GLContext context) {
    imgui_context_ = ImGui::CreateContext();
    if (!imgui_context_) {
        Log::error("Could not create imgui context");
        return;
    }
    dispatcher_.sink<PostRenderEvent>().connect<&Xray::on_post_render>(this);

    ImGui_ImplSDL2_InitForOpenGL(window_, context);
    ImGui_ImplOpenGL3_Init();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

void Xray::shutdown() {
    if (imgui_context_) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext(imgui_context_);
        imgui_context_ = nullptr;
    }
}

void Xray::on_post_render(const PostRenderEvent&) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame(window_.handle());
    ImGui::NewFrame();

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2 { 0.0f, 0.0f });

    constexpr auto window_flags = 
        ImGuiWindowFlags_NoDocking |
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoBringToFrontOnFocus |
        ImGuiWindowFlags_NoNavFocus;

    ImGui::Begin("ImGuiMainWindow", nullptr, window_flags);
    ImGui::PopStyleVar(1);

    const auto dock_id { ImGui::GetID("XrayDock") };
    ImGui::DockSpace(dock_id, ImVec2{}, ImGuiDockNodeFlags_PassthruCentralNode);

    for (const auto& xray : xrays_) {
        xray->update();
    }

    ImGui::End();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    const bool any_active { ImGui::IsAnyItemActive() };

    if (focused_ && !any_active) {
        dispatcher_.trigger<XrayFocusEvent>(false);
        focused_ = false;
    } else if (!focused_ && any_active) {
        dispatcher_.trigger<XrayFocusEvent>(true);
        focused_ = true;
    }
}
