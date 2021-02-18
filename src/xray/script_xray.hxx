#ifndef JARALYN_SCRIPT_XRAY_HXX
#define JARALYN_SCRIPT_XRAY_HXX

#include "xray_interface.hxx"
#include "xray_style.hxx"
#include "../game.hxx"
#include "../scripts/script_system.hxx"
#include "../hud/hud_system.hxx"

class j_script_xray : public j_xray_interface {
public:
    void update() override;
private:
    j_id_t script_id_ { j_id_invalid };
};

#endif
