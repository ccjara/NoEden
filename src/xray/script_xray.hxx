#ifndef NOEDEN_SCRIPT_XRAY_HXX
#define NOEDEN_SCRIPT_XRAY_HXX

#include "xray/xray_interface.hxx"
#include "xray/xray_style.hxx"

class Scripting;
class Script;
struct ScriptLoadedEvent;
struct ScriptResetEvent;

class ScriptXray : public IXray {
public:
    explicit ScriptXray(Scripting* scripting, EventManager* events);

    void update() override;
private:
    Scripting* scripting_ = nullptr;

    void render_current_script(Script *current);

    bool on_script_loaded(ScriptLoadedEvent& e);
    bool on_script_reset(ScriptResetEvent& e);

    struct GlobalsState {
        bool visible = false;
        int ticks = 0;
        std::vector<std::tuple<std::string, std::string, std::string>> rows;
    } globals_;

    // ui vars
    std::optional<u64> selected_script_id_;
    Script* current_script_;

    void update_script_globals_table();
};

#endif
