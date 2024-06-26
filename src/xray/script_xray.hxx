#ifndef NOEDEN_SCRIPT_XRAY_HXX
#define NOEDEN_SCRIPT_XRAY_HXX

#include "scripts/script_event.hxx"
#include "xray/xray_interface.hxx"
#include "xray/xray_style.hxx"

class Scripting;
class Script;

class ScriptXray : public IXray {
public:
    explicit ScriptXray(Scripting* scripting, EventManager* events);

    void update() override;
private:
    Scripting* scripting_ = nullptr;

    void render_current_script(Script *current);

    EventResult on_script_reset(ScriptResetEvent& e);
    Subscription<ScriptResetEvent> script_reset_sub_;

    struct GlobalsState {
        bool visible = false;
        int ticks = 0;
        std::vector<std::tuple<std::string, std::string, std::string>> rows;
    } globals_;

    // ui vars
    std::optional<u64> selected_script_id_;
    Script* current_script_ = nullptr;

    void update_script_globals_table();
};

#endif
