#ifndef JARALYN_AI_NODE_HXX
#define JARALYN_AI_NODE_HXX

enum class AiNodeState {
    Ready,
    Running,
    Failed,
    Succeeded,
};

/**
 * @brief Base class for every behavior tree node, abbrev. as 'Ai'
 * 
 * The current implementation is probably a cache trasher and has lots of 
 * indirections, but it will do fine for now.
 */
class AiNode {
public:
    AiNode* parent_ { this };

    virtual ~AiNode() = default;

    /**
     * @brief Visits and runs a node. The behavior is implementation defined.
     */
    virtual AiNodeState visit() = 0;

    /**
     * @brief The implementation
     */
    virtual void clear() = 0;

    constexpr AiNodeState state() const {
        return state_;
    };

    constexpr bool failed() const {
        return state_ == AiNodeState::Failed;
    }
protected:
    constexpr AiNodeState mod_state(AiNodeState s) {
        state_ = s;
        return s;
    }
private:
    AiNodeState state_ { AiNodeState::Ready };
};

/**
 * @brief AI Selector implementation using prioritized nodes
 * 
 * This node fails if no child node produced a state other than `failed`.
 */
class AiPrioritySelector : public AiNode {
public:
    /**
     * @brief Traverses the prioritized list, visiting child nodes in order.
     * 
     * The nodes are visited based on their priority, higher prioritized nodes
     * coming first. If a previous visit caused a node to switch from the
     * `ready` to `running` state, the `continuation_iterator` is updated so
     * execution can continue immediately on the next visit.
     */
    AiNodeState visit() override {
        while (continuation_iterator != nodes_.end()) {
            const auto child_state { continuation_iterator->ptr->visit() };

            if (child_state == AiNodeState::Failed) {
                // continue iteration, trying the next node available
                continue;
            } else {
                // preserve the continuation iterator and return the child node's result
                return mod_state(child_state);
            }
        }
        // fail as no child node resulted in a non-failure state
        return mod_state(AiNodeState::Failed);
    }

    /**
     * @brief Constructs and returns a node of the given type in a prioritized list.
     * 
     * A higher priority causes the node to be visited before others.
     * 
     * Clears the state of all child nodes on each invocation.
     */
    template<typename Node, typename... NodeArgs>
    Node& add(i32 priority, NodeArgs&&... args) {
        // inefficient, but shouldn't get us into trouble unless we construct
        // massive behavior trees
        // reset state of all child nodes
        clear();

        auto node_ptr { new Node(std::forward<NodeArgs>(args)...) };
        node_ptr->parent_ = this;
        auto& node { nodes_.emplace_back(node_ptr, priority) };
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
        return *node_ptr;
    }

    /**
     * @brief Clears all child nodes and resets the continuation iterator
     */
    void clear() override {
        for (auto& n : nodes_) {
            n.ptr->clear();
        }
        continuation_iterator = nodes_.begin();
    }
private:
    struct PrioritizedNode {
        std::unique_ptr<AiNode> ptr { nullptr };
        i32 priority { 0 };

        PrioritizedNode(AiNode* ptr, i32 priority = 0) :
            ptr { ptr },
            priority { priority } {
            assert(this->ptr);
        }
    };
    
    std::vector<PrioritizedNode> nodes_;
    std::vector<PrioritizedNode>::iterator continuation_iterator { nodes_.begin() };
};

class AiSequenceSelector : AiNode {
    // TODO!
};

class AiCondition : AiNode {
};

#endif
