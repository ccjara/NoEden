#include "xray/xray_manager.hxx"
#include "xray/xray.hxx"

XrayManager::XrayManager(EventManager* events) : events_(events) {
    assert(events);

    mouse_down_sub_ = events->on<MouseDownEvent>(this, &XrayManager::on_mouse_down, 10000);
    mouse_up_sub_ = events->on<MouseUpEvent>(this, &XrayManager::on_mouse_up, 10000);
    key_down_sub_ = events->on<KeyDownEvent>(this, &XrayManager::on_key_down, 10000);
    key_up_sub_ = events->on<KeyUpEvent>(this, &XrayManager::on_key_up, 10000);

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
}

XrayManager::~XrayManager() {
    xrays_.clear();

    mouse_down_sub_.unsubscribe();
    mouse_up_sub_.unsubscribe();
    key_down_sub_.unsubscribe();
    key_up_sub_.unsubscribe();
}


void XrayManager::render() {
    if (!show_xray_) {
        ImGui::Render();
        return;
    }

    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0.0f, 0.0f});

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

    const auto dock_id{ImGui::GetID("XrayDock")};
    ImGui::DockSpace(dock_id, ImVec2{}, ImGuiDockNodeFlags_PassthruCentralNode);

    for (const auto& xray : xrays_) {
        xray->render();
    }

    ImGui::End();
    ImGui::Render();
}

void XrayManager::add_xray(std::unique_ptr<Xray>&& xray) {
    xrays_.push_back(std::move(xray));
}

EventResult XrayManager::on_mouse_down(const MouseDownEvent& e) {
    return ImGui::GetIO().WantCaptureMouse
               ? EventResult::Halt
               : EventResult::Continue;
}

EventResult XrayManager::on_mouse_up(const MouseUpEvent& e) {
    return ImGui::GetIO().WantCaptureMouse
               ? EventResult::Halt
               : EventResult::Continue;
}

EventResult XrayManager::on_key_down(const KeyDownEvent& e) {
    if (ImGui::GetIO().WantCaptureKeyboard) {
        return EventResult::Halt;
    }
    if (e.key == Key::F1) {
        show_xray_ = !show_xray_;
        return EventResult::Halt;
    }
    return EventResult::Continue;
}

EventResult XrayManager::on_key_up(const KeyUpEvent& e) {
    return ImGui::GetIO().WantCaptureKeyboard
               ? EventResult::Halt
               : EventResult::Continue;
}
