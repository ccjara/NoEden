#include "ai_priority_selector.hxx"

AiPrioritySelector& AiPrioritySelector::operator=(AiPrioritySelector other) {
    swap(*this, other);
    return *this;
}

AiPrioritySelector::AiPrioritySelector(const AiPrioritySelector& other) {
    nodes_ = other.nodes_;
    continuation_iterator = nodes_.begin();
}

AiPrioritySelector::AiPrioritySelector(AiPrioritySelector&& other) : AiPrioritySelector() {
    swap(*this, other);
}

AiNodeState AiPrioritySelector::visit(AiContext& context) {
    mod_state(AiNodeState::Running);
    continuation_iterator = nodes_.begin();

    while (continuation_iterator != nodes_.end()) {
        assert(continuation_iterator->ptr);
        const auto child_state = continuation_iterator->ptr->visit(context);

        if (child_state == AiNodeState::Failed) {
            // continue iteration, trying the next node available
            ++continuation_iterator;
            continue;
        } else {
            // preserve the continuation iterator and return the child node's result
            return mod_state(child_state);
        }
    }
    // fail as no child node resulted in a non-failure state
    return mod_state(AiNodeState::Failed);
}

void AiPrioritySelector::clear() {
    for (auto& n : nodes_) {
        n.ptr->clear();
    }
    continuation_iterator = nodes_.begin();
}

void AiPrioritySelector::post_create_node() {
    // inefficient, but shouldn't get us into trouble unless we construct
    // massive behavior trees
    // reset state of all child nodes
    clear();
    // enter poor man's priority queue
    std::sort(
        nodes_.begin(),
        nodes_.end(),
        [](const auto& a, const auto& b) -> bool {
            return a.priority > b.priority;
        }
    );
    // iterator must be invalidated as elements might get reordered
    continuation_iterator = nodes_.begin();
}


// -- PrioritizedNode

// default
AiPrioritySelector::PrioritizedNode::PrioritizedNode(std::unique_ptr<AiNode>&& ptr, i32 priority) :
    ptr(std::move(ptr)),
    priority(priority) {
    assert(this->ptr);
}

// copy
AiPrioritySelector::PrioritizedNode::PrioritizedNode(const AiPrioritySelector::PrioritizedNode& other) {
    ptr = other.ptr->clone();
    priority = other.priority;
}

// move
AiPrioritySelector::PrioritizedNode::PrioritizedNode(AiPrioritySelector::PrioritizedNode&& other) : AiPrioritySelector::PrioritizedNode() {
    swap(*this, other);
}

// cas
AiPrioritySelector::PrioritizedNode& AiPrioritySelector::PrioritizedNode::operator=(AiPrioritySelector::PrioritizedNode other) {
    swap(*this, other);
    return *this;
}
