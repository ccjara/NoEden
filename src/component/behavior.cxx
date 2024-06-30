#include "component/behavior.hxx"
#include "entity/entity.hxx"
#include "ai/ai_node.hxx"

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

void Behavior::update() {
    if (!root_) {
        return;
    }
    if (entity_->energy_reserved >= entity_->energy) {
        return;
    }
    root_->visit(ai_context_);
}

void Behavior::on_owner_updated() {
    assert(entity_);
    ai_context_.entity = entity_;
    ai_context_.entity_id = entity_->id;
    ai_context_.blackboard.clear();
    root_->initialize(ai_context_);
}

void Behavior::set_root(std::unique_ptr<AiNode>&& root) {
    root_ = std::move(root);
}

AiNode* Behavior::root() const {
    return root_.get();
}
