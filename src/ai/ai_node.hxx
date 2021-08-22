#ifndef JARALYN_AI_NODE_HXX
#define JARALYN_AI_NODE_HXX

enum class j_ai_node_state {
    ready,
    running,
    failed,
    succeeded,
};

/**
 * @brief Base class for every behavior tree node.
 * 
 * The current implementation is a cache trasher and has lots of indirections, 
 * but it will do fine for now.
 */
class j_ai_node {
public:
    j_ai_node* parent_ { this };

    /**
     * @brief Visits and runs a node. The behavior is implementation defined.
     */
    virtual j_ai_node_state visit() = 0;

    /**
     * @brief The implementation
     */
    virtual void clear() = 0;

    constexpr j_ai_node_state state() const {
        return state_;
    };

    constexpr bool failed() const {
        return state_ == j_ai_node_state::failed;
    }
protected:
    constexpr j_ai_node_state mod_state(j_ai_node_state s) {
        state_ = s;
        return s;
    }
private:
    j_ai_node_state state_ { j_ai_node_state::ready };
};

/**
 * @brief AI Selector implementation using prioritized nodes
 * 
 * This node fails if no child node produced a state other than `failed`.
 */
class j_ai_priority_selector : public j_ai_node {
public:
    /**
     * @brief Traverses the prioritized list, visiting child nodes in order.
     * 
     * The nodes are visited based on their priority, higher prioritized nodes
     * coming first. If a previous visit caused a node to switch from the
     * `ready` to `running` state, the `continuation_iterator` is updated so
     * execution can continue immediately on the next visit.
     */
    j_ai_node_state visit() override {
        while (continuation_iterator != nodes_.end()) {
            const auto child_state { continuation_iterator->ptr->visit() };

            if (child_state == j_ai_node_state::failed) {
                // continue iteration, trying the next node available
                continue;
            } else {
                // preserve the continuation iterator and return the child node's result
                return mod_state(child_state);
            }
        }
        // fail as no child node resulted in a non-failure state
        return mod_state(j_ai_node_state::failed);
    }

    /**
     * @brief Constructs and returns a node of the given type in a prioritized list.
     * 
     * A higher priority causes the node to be visited before others.
     * 
     * Clears the state of all child nodes on each invocation.
     */
    template<typename ai_node, typename... node_args>
    ai_node& add(int32_t priority, node_args&&... args) {
        // inefficient, but shouldn't get us into trouble unless we construct
        // massive behavior trees
        // reset state of all child nodes
        clear();

        auto node_ptr { new ai_node(std::forward<node_args>(args)...) };
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
    struct prioritized_node {
        std::unique_ptr<j_ai_node> ptr { nullptr };
        int32_t priority { 0 };

        prioritized_node(j_ai_node* ptr, int32_t priority = 0) :
            ptr { ptr },
            priority { priority } {
            assert(this->ptr);
        }
    };
    
    std::vector<prioritized_node> nodes_;
    std::vector<prioritized_node>::iterator continuation_iterator { nodes_.begin() };
};

class j_ai_sequence_selector : j_ai_node {
    // TODO!
};

class j_ai_condition : j_ai_node {
    // TODO!
};

class j_ai_action : public j_ai_node {
    // TODO!
public:
    j_ai_node_state visit() override {
        LOG(INFO) << "AI action";

        return mod_state(j_ai_node_state::succeeded);
    }

    void clear() override {

    }
};

#endif
