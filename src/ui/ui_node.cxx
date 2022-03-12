#include "ui_node.hxx"

void UiNode::move_anchors() {
    for (const auto n : anchored_by_) {
        if (!n) {
            continue;
        }
        n->move_abs(n->relative_position_ + calc_anchor_offset(*n, *this));
    }
}

void UiNode::move(Vec2<u32> position) {
    relative_position_ = position;
    if (anchored_to_) {
        position += calc_anchor_offset(*this, *anchored_to_);
    }
    move_abs(position);
}

void UiNode::move_abs(Vec2<u32> pos) {
    absolute_position_ = pos;
    move_anchors();
}

void UiNode::set_anchor_origin(AnchorOrigin origin) {
    anchor_origin_ = origin;
    move(relative_position_);
}

void UiNode::resize(Vec2<u32> size) {
    size_ = size;
    // anchored nodes may update their positions due to their anchor origin
    move_anchors();
}

void UiNode::anchor_to(UiNode& node) {
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

void UiNode::call_handler() {
    if (handler_) {
        handler_.value()(this);
    }
}

void UiNode::set_handler(luabridge::LuaRef ref) {
    if (ref.isCallable()) {
        handler_ = ref;
    }
}

const std::string& UiNode::id() const {
    return id_;
}

UiNodeType UiNode::type() const {
    return type_;
}

Vec2<u32> UiNode::relative_position() const {
    return relative_position_;
}

Vec2<u32> UiNode::absolute_position() const {
    return absolute_position_;
}

UiNode* UiNode::anchored_to() const {
    return anchored_to_;
}

AnchorOrigin UiNode::anchor_origin() const {
    return anchor_origin_;
}

const std::vector<UiNode*>& UiNode::children() const {
    return children_;
}

const std::vector<UiNode*>& UiNode::anchored_by() const {
    return anchored_by_;
}

Vec2<u32> UiNode::size() const {
    return size_;
}

bool UiNode::is_root() const {
    return parent_ == nullptr;
}

bool UiNode::can_anchor_to(UiNode* node) const {
    return !is_root() && node != this;
}

bool UiNode::visible() const {
    return visible_;
}

void UiNode::show() {
    visible_ = true;
}

void UiNode::hide() {
    visible_ = false;
}

Vec2<u32> calc_anchor_offset(
    const UiNode& target,
    const UiNode& anchor
) {
    switch (target.anchor_origin()) {
        case AnchorOrigin::TopLeft:
        default:
            return anchor.absolute_position();
        case AnchorOrigin::Top:
            return {
                (anchor.absolute_position().x + anchor.size().x) / 2,
                anchor.absolute_position().y
            };
        case AnchorOrigin::TopRight:
            return {
                anchor.absolute_position().x + anchor.size().x,
                anchor.absolute_position().y
            };
        case AnchorOrigin::Right:
            return {
                anchor.absolute_position().x + anchor.size().x,
                (anchor.absolute_position().y + anchor.size().y) / 2,
            };
        case AnchorOrigin::BottomRight:
            return anchor.absolute_position() + anchor.size();
        case AnchorOrigin::Bottom:
            return {
                (anchor.absolute_position().x + anchor.size().x) / 2,
                anchor.absolute_position().y + anchor.size().y,
            };
        case AnchorOrigin::BottomLeft:
            return {
                anchor.absolute_position().x,
                anchor.absolute_position().y + anchor.size().y,
            };
        case AnchorOrigin::Left:
            return {
                anchor.absolute_position().x,
                (anchor.absolute_position().y + anchor.size().y) / 2,
            };
        case AnchorOrigin::Center:
            return (anchor.absolute_position() + anchor.size()) / 2u;
    }
}
