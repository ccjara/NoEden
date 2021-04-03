#include "ui_node.hxx"

void j_ui_node::move_anchors() {
    for (const auto n : anchored_by_) {
        if (!n) {
            continue;
        }
        n->move_abs(n->relative_position_ + calc_anchor_offset(*n, *this));
    }
}

void j_ui_node::move(j_vec2<uint32_t> position) {
    relative_position_ = position;
    if (anchored_to_) {
        position += calc_anchor_offset(*this, *anchored_to_);
    }
    move_abs(position);
}

void j_ui_node::move_abs(j_vec2<uint32_t> pos) {
    absolute_position_ = pos;
    move_anchors();
}

void j_ui_node::set_anchor_origin(j_ui_anchor_origin origin) {
    anchor_origin_ = origin;
    move(relative_position_);
}

void j_ui_node::resize(j_vec2<uint32_t> size) {
    size_ = size;
    // anchored nodes may update their positions due to their anchor origin
    move_anchors();
}

void j_ui_node::anchor_to(j_ui_node& node) {
    if (!can_anchor_to(&node)) {
        return;
    }
    if (anchored_to_ && anchored_to_ != this) {
        // remove this node from the current anchor's list
        auto& foreign_anchors { anchored_to_->anchored_by_ };
        for (auto iter = foreign_anchors.begin(); iter != foreign_anchors.end(); ++iter) {
            if (*iter == this) {
                iter = foreign_anchors.erase(iter);
                break;
            }
        }
    }
    // assign new references on both sides
    node.anchored_by_.emplace_back(this);
    anchored_to_ = &node;
    // update position after anchor was changed
    move(relative_position_);
}

const std::string& j_ui_node::id() const {
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

j_ui_node* j_ui_node::anchored_to() const {
    return anchored_to_;
}

j_ui_anchor_origin j_ui_node::anchor_origin() const {
    return anchor_origin_;
}

const std::vector<j_ui_node*>& j_ui_node::children() const {
    return children_;
}

const std::vector<j_ui_node*>& j_ui_node::anchored_by() const {
    return anchored_by_;
}

j_vec2<uint32_t> j_ui_node::size() const {
    return size_;
}

bool j_ui_node::is_root() const {
    return parent_ == nullptr;
}

bool j_ui_node::can_anchor_to(j_ui_node* node) const {
    return !is_root() && node != this;
}

bool j_ui_node::visible() const {
    return visible_;
}

void j_ui_node::show() {
    visible_ = true;
}

void j_ui_node::hide() {
    visible_ = false;
}

j_vec2<uint32_t> calc_anchor_offset(
    const j_ui_node& target,
    const j_ui_node& anchor
) {
    switch (target.anchor_origin()) {
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
