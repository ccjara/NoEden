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
                        case AnchorOrigin::Center:
                        case AnchorOrigin::Top:
                        case AnchorOrigin::TopRight:
                        case AnchorOrigin::Right:
                        case AnchorOrigin::BottomRight:
                        case AnchorOrigin::Bottom:
                        case AnchorOrigin::BottomLeft:
                        case AnchorOrigin::Left:
                        case AnchorOrigin::TopLeft:
                            n->set_anchor_origin(unsafe_origin);
                            return;
                        default:
                            Log::error(
                                "Unknown anchor origin {}, please use one of "
                                "the following: AnchorOrigin.Top, AnchorOrigin.TopRight, "
                                "AnchorOrigin.Right, AnchorOrigin.BottomRight, "
                                "AnchorOrigin.Bottom, AnchorOrigin.BottomLeft, "
                                "AnchorOrigin.Left, AnchorOrigin.TopLeft or "
                                "AnchorOrigin.Center.",
                                origin
                            );
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