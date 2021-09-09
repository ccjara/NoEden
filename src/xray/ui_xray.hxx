#ifndef JARALYN_UI_XRAY_HXX
#define JARALYN_UI_XRAY_HXX

#include "xray_interface.hxx"
#include "xray_style.hxx"
#include "../ui/ui.hxx"

class UiXray : public IXray {
public:
    explicit UiXray(entt::dispatcher& dispatcher, Ui& ui);

    void update() override;
private:
    Ui& ui_;
    entt::dispatcher& dispatcher_;
};

#endif
