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

    std::string selected_node_id_;

    bool render_anchor(UiNode* node);
    bool render_hierarchy(UiNode* node);
};

#endif
