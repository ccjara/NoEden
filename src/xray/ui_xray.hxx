#ifndef NOEDEN_UI_XRAY_HXX
#define NOEDEN_UI_XRAY_HXX

#include "xray/xray.hxx"

class UiNode;

class UiXray : public Xray {
public:
    void render() override;
private:
    std::string selected_node_id_;

    bool render_anchor(UiNode* node);
    bool render_hierarchy(UiNode* node);
};

#endif
