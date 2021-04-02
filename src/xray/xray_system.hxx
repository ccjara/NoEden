#ifndef JARALYN_XRAY_SYSTEM_HXX
#define JARALYN_XRAY_SYSTEM_HXX

#include "../system.hxx"
#include "../game.hxx"
#include "../gfx/gfx_system.hxx"
#include "../event/gfx_event.hxx"
#include "script_xray.hxx"
#include "ui_xray.hxx"

class j_xray_system : public j_system<j_xray_system> {
public:
    void on_load() override;
    void on_unload() override;
    void update(uint32_t delta_time) override;
private:
    void immediate_on_post_render(const j_post_render_event& e);

    ImGuiContext* imgui_context_ { nullptr };
    j_window* window_ { nullptr };

    std::vector<std::unique_ptr<j_xray_interface>> xrays_;
};

#endif
