#ifndef JARALYN_UI_XRAY_HXX
#define JARALYN_UI_XRAY_HXX

#include "xray_interface.hxx"
#include "xray_style.hxx"
#include "../game.hxx"
#include "../hud/hud_system.hxx"

class j_ui_xray : public j_xray_interface {
public:
    void update() override;
private:
};

#endif
