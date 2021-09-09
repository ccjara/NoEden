#ifndef JARALYN_SCRIPT_XRAY_HXX
#define JARALYN_SCRIPT_XRAY_HXX

#include "xray_interface.hxx"
#include "xray_style.hxx"
#include "../scripts/Scripts.hxx"

class ScriptXray : public IXray {
public:
    explicit ScriptXray(entt::dispatcher& dispatcher, Scripts& scripts);

    void update() override;
private:
    Scripts& scripts_;
    entt::dispatcher& dispatcher_;

    void render_current_script(Script *current);

    void on_script_loaded(const ScriptLoadedEvent& e);
    void on_script_reset(const ScriptResetEvent& e);

    // ui vars
    std::optional<u64> selected_script_id_;
};

#endif
