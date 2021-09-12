#include "ui_api.hxx"

UiApi::UiApi(UiTree& ui_tree) : ui_tree_ { ui_tree } {

}

void UiApi::on_register(Script* script) {
    luabridge::getGlobalNamespace(*script)
        .beginClass<UiApi>("UiApi")
            .addFunction("create_window", &UiApi::create_window)
        .endClass()
        // nodes
        .beginClass<UiNode>("UiNode")
            .addFunction(
                "set_anchor_origin",
                +[](UiNode* n, u32 origin) {
                    const auto unsafe_origin { static_cast<AnchorOrigin> (origin) };

                    switch (unsafe_origin) {
                        case AnchorOrigin::center:
                        case AnchorOrigin::top:
                        case AnchorOrigin::top_right:
                        case AnchorOrigin::right:
                        case AnchorOrigin::bottom_right:
                        case AnchorOrigin::bottom:
                        case AnchorOrigin::bottom_left:
                        case AnchorOrigin::left:
                        case AnchorOrigin::top_left:
                            n->set_anchor_origin(unsafe_origin);
                            return;
                        default:
                            LOG(ERROR) << "Unknown anchor origin " << origin
                                    << ". Please use one of the following: "
                                        "AnchorOrigin.top, AnchorOrigin.top_right, "
                                        "AnchorOrigin.right, AnchorOrigin.bottom_right, "
                                        "AnchorOrigin.bottom, AnchorOrigin.bottom_left, "
                                        "AnchorOrigin.left, AnchorOrigin.top_left or "
                                        "AnchorOrigin.center.";
                    }
                }
            )
            .addFunction("set_handler", 
                +[](UiNode* n, luabridge::LuaRef handler) {
                    return n->set_handler(handler);
                }
            )
            .addFunction(
                "move",
                +[](UiNode* n, u32 x, u32 y) { n->move({ x, y }); }
            )
            .addFunction(
                "resize",
                +[](UiNode* n, u32 x, u32 y) { n->resize({ x, y }); }
            )
            .addFunction("show", +[](UiNode* n) { n->show(); })
            .addFunction("hide", +[](UiNode* n) { n->hide(); })
        .endClass()
        .deriveClass<UiWindow, UiNode>("UiWindow")
            .addFunction(
                "set_title",
                +[](UiWindow* n, const char *title) { n->set_title(title); }
            )
        .endClass();

    expose(script, this, "ui");
}

UiWindow* UiApi::create_window(const char* id) {
    auto window { ui_tree_.create_node<UiWindow>(nullptr, id) };
    if (!window) {
        return nullptr;
    }
    return window;
}