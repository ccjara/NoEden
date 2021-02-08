#include "ui_node.hxx"

void j_ui_node::move(j_vec2<uint32_t> position) {
    relative_position_ = position;
    if (anchor_.node) {
        position += calc_anchor_offset(*this, *anchor_.node);
    }
    move_abs(position);
}

void j_ui_node::move_abs(j_vec2<uint32_t> pos) {
    absolute_position_ = pos;
    for (const auto n : anchor_.nodes) {
        n->move_abs(n->relative_position_ + calc_anchor_offset(*n, *this));
    }
}

void j_ui_node::set_anchor_origin(j_ui_anchor_origin origin) {
    anchor_.origin = origin;
    move(relative_position_);
}

void j_ui_node::resize(j_vec2<uint32_t> size) {
    size_ = size;
}

std::string_view j_ui_node::id() const {
    return id_;
}

j_ui_node_type j_ui_node::type() const {
    return type_;
}

j_vec2<uint32_t> j_ui_node::relative_position() const {
    return relative_position_;
}

j_vec2<uint32_t> j_ui_node::absolute_position() const {
    return absolute_position_;
}

const j_ui_node::j_ui_anchor& j_ui_node::anchor() const {
    return anchor_;
}

const std::vector<j_ui_node*>& j_ui_node::children() const {
    return children_;
}

j_vec2<uint32_t> j_ui_node::size() const {
    return size_;
}

j_vec2<uint32_t> calc_anchor_offset(
    const j_ui_node& target,
    const j_ui_node& anchor
) {
    switch (target.anchor().origin) {
        case j_ui_anchor_origin::top_left:
        default:
            return anchor.absolute_position();
        case j_ui_anchor_origin::top:
            return { 
                (anchor.absolute_position().x + anchor.size().x) / 2,
                anchor.absolute_position().y
            };
        case j_ui_anchor_origin::top_right:
            return {
                anchor.absolute_position().x + anchor.size().x,
                anchor.absolute_position().y
            };
        case j_ui_anchor_origin::right:
            return {
                anchor.absolute_position().x + anchor.size().x,
                (anchor.absolute_position().y + anchor.size().y) / 2,
            };
        case j_ui_anchor_origin::bottom_right:
            return anchor.absolute_position() + anchor.size();
        case j_ui_anchor_origin::bottom:
            return {
                (anchor.absolute_position().x + anchor.size().x) / 2,
                anchor.absolute_position().y + anchor.size().y,
            };
        case j_ui_anchor_origin::bottom_left:
            return {
                anchor.absolute_position().x,
                anchor.absolute_position().y + anchor.size().y,
            };
        case j_ui_anchor_origin::left:
            return {
                anchor.absolute_position().x,
                (anchor.absolute_position().y + anchor.size().y) / 2,
            };
        case j_ui_anchor_origin::center:
            return (anchor.absolute_position() + anchor.size()) / 2u;
    }
}