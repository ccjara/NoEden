#ifndef NOEDEN_UI_XRAY_HXX
#define NOEDEN_UI_XRAY_HXX

#include "xray_interface.hxx"
#include "xray_style.hxx"
#include "ui/ui.hxx"

class UiXray : public IXray {
public:
    void update() override;
private:
    std::string selected_node_id_;

    bool render_anchor(UiNode* node);
    bool render_hierarchy(UiNode* node);
};

#endif
