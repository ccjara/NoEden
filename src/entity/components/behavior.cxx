#include "behavior.hxx"

Behavior& Behavior::operator=(Behavior other) {
    swap(*this, other);
    return *this;
}

Behavior::Behavior(const Behavior& other) {
    if (other.root_) {
        root_ = other.root_->clone();
    }
}

Behavior::Behavior(Behavior&& other) : Behavior() {
    swap(*this, other);
}

void Behavior::update(u64 dt) {
    if (!root_) {
        return;
    }
    //do {
        root_->visit(ai_context_);
    // } while (root_->state() != AiNodeState::Failed);
}

void Behavior::set_owner(Entity* entity) {
    if (!entity) {
        return;
    }
    ai_context_.entity = entity;
    ai_context_.entity_id = entity->id;
    ai_context_.blackboard.clear();
}

void Behavior::set_root(std::unique_ptr<AiNode>&& root) {
    root_ = std::move(root);
}

AiNode* Behavior::root() const {
    return root_.get();
}