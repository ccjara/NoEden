#include "render.hxx"

Render::Render() : Component(ComponentType::Render) {
}

const DisplayInfo& Render::display_info() const {
    return display_info_;
}

DisplayInfo& Render::display_info() {
    return display_info_;
}

std::unique_ptr<Component> Render::clone() const {
    return std::make_unique<Render>(*this);
}
