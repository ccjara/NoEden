#ifndef JARALYN_SCRIPT_XRAY_HXX
#define JARALYN_SCRIPT_XRAY_HXX

#include "xray_interface.hxx"
#include "xray_style.hxx"
#include "../scripts/scripting.hxx"

class ScriptXray : public IXray {
public:
    ScriptXray();

    void update() override;
private:
    void render_current_script(Script *current);

    bool on_script_loaded(ScriptLoadedEvent& e);
    bool on_script_reset(ScriptResetEvent& e);

    // ui vars
    std::optional<u64> selected_script_id_;
};

#endif
