#ifndef JARALYN_UI_XRAY_HXX
#define JARALYN_UI_XRAY_HXX

#include "xray_interface.hxx"
#include "xray_style.hxx"
#include "../ui/ui.hxx"

class UiXray : public IXray {
public:
    explicit UiXray(EventManager& dispatcher, Ui& ui);

    void update() override;
private:
    Ui& ui_;
    EventManager& events_;
};

#endif
