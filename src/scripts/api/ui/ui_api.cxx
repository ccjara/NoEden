#include "ui_api.hxx"
#include "ui/ui.hxx"
#include "ui/ui_window.hxx"
#include "ui/ui_text.hxx"

bool UiApi::initialize() {
    ui_ = svc_->get<Ui>();

    if (!ui_) {
        LOG_ERROR("UiApi failed to initialize: failed to get Ui");
        return false;
    }

    return true;
}

void UiApi::on_register(Script& script) {
    script.define_enum(
        "AlignX", 
        std::make_tuple("Left", AlignX::Left), 
        std::make_tuple("Center", AlignX::Center), 
        std::make_tuple("Right", AlignX::Right)
    );

    script.define_enum(
        "AlignY", 
        std::make_tuple("Top", AlignY::Top), 
        std::make_tuple("Center", AlignY::Center), 
        std::make_tuple("Bottom", AlignY::Bottom)
    );

    script.define_enum(
        "AnchorOrigin", 
        std::make_tuple("Top", AnchorOrigin::Top),
        std::make_tuple("TopRight", AnchorOrigin::TopRight),
        std::make_tuple("Right", AnchorOrigin::Right),
        std::make_tuple("BottomRight", AnchorOrigin::BottomRight),
        std::make_tuple("Bottom", AnchorOrigin::Bottom),
        std::make_tuple("BottomLeft", AnchorOrigin::BottomLeft),
        std::make_tuple("Left", AnchorOrigin::Left),
        std::make_tuple("TopLeft", AnchorOrigin::TopLeft),
        std::make_tuple("Center", AnchorOrigin::Center)
    );

    luabridge::getGlobalNamespace(script)
        .beginClass<UiApi>("UiApi")
            .addFunction("create_window", &UiApi::create_window)
            .addFunction("create_text", &UiApi::create_text)
            .addFunction("destroy_node", &UiApi::destroy_node)
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
                            LOG_ERROR(
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
            .addFunction("set_parent", +[](UiNode* n, UiNode* target) {
                n->set_parent(target);
            })
            .addFunction("anchor_to", +[](UiNode* n, UiNode* target) {
                n->anchor_to(target);
            })
            .addFunction("set_align_x", +[](UiNode* n, u32 align_x) {
                const auto unsafe_align_x { static_cast<AlignX> (align_x) };
                switch (unsafe_align_x) {
                    case AlignX::Left:
                    case AlignX::Center:
                    case AlignX::Right:
                        n->set_align_x(unsafe_align_x);
                        break;
                    default:
                        LOG_ERROR("Unknown align_x {}, please use AlignX.Left, AlignX.Center or AlignX.Right", align_x);
                }
            })
            .addFunction("set_align_y", +[](UiNode* n, u32 align_y) {
                const auto unsafe_align_y { static_cast<AlignY> (align_y) };
                switch (unsafe_align_y) {
                    case AlignY::Top:
                    case AlignY::Center:
                    case AlignY::Bottom:
                        n->set_align_y(unsafe_align_y);
                        break;
                    default:
                        LOG_ERROR("Unknown align_y {}, please use AlignY.Top, AlignY.Center or AlignY.Bottom", align_y);
                }
            })
            .addFunction("set_handler",
                +[](UiNode* n, luabridge::LuaRef handler) {
                    return n->set_handler(handler);
                }
            )
            .addFunction(
                "move",
                +[](UiNode* n, i32 x, i32 y) { n->move({ x, y }); }
            )
            .addFunction(
                "resize",
                +[](UiNode* n, u32 x, u32 y) {
                    n->resize(x, y);
                }
            )
            .addFunction("show", +[](UiNode* n) { n->show(); })
            .addFunction("hide", +[](UiNode* n) { n->hide(); })
        .endClass()
        .deriveClass<UiWindow, UiNode>("UiWindow")
            .addFunction(
                "set_title",
                +[](UiWindow* n, const char *title) { n->set_title(title); }
            )
        .endClass()
        .deriveClass<UiText, UiNode>("UiText")
            .addFunction(
                "set_text",
                +[](UiText* n, const char *text) {
                    n->text().set_text(text);
                    n->text().update();
                }
            )
        .endClass();

    expose(script, this, "ui");
}

UiWindow* UiApi::create_window(const char* id) {
    if (!id) {
        return nullptr;
    }
    return ui_->tree().create_node<UiWindow>(nullptr, id);
}

UiText* UiApi::create_text(const char* id) {
    return ui_->tree().create_node<UiText>(nullptr, id);
}

void UiApi::destroy_node(const char* id) {
    if (!id) {
        return;
    }
    ui_->tree().remove_node(id);
}
