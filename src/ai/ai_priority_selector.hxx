#ifndef JARALYN_AI_PRIORITY_SELECTOR_HXX
#define JARALYN_AI_PRIORITY_SELECTOR_HXX

#include "generic_ai_node.hxx"

/**
 * @brief AI Selector implementation using prioritized nodes
 *
 * This node fails if no child node produced a state other than `failed`.
 */
class AiPrioritySelector : public GenericAiNode<AiPrioritySelector> {
public:
    AiPrioritySelector() = default;
    AiPrioritySelector(const AiPrioritySelector& other);
    AiPrioritySelector(AiPrioritySelector&& other);
    AiPrioritySelector& operator=(AiPrioritySelector other);

    friend void swap(AiPrioritySelector& lhs, AiPrioritySelector& rhs) {
        using std::swap;
        swap(lhs.nodes_, rhs.nodes_);
        lhs.nodes_.swap(rhs.nodes_);
        lhs.continuation_iterator = lhs.nodes_.begin();
        rhs.continuation_iterator = rhs.nodes_.begin();
    }

    /**
     * @brief Traverses the prioritized list, visiting child nodes in order.
     *
     * The nodes are visited based on their priority, higher prioritized nodes
     * coming first. If a previous visit caused a node to switch from the
     * `ready` to `running` state, the `continuation_iterator` is updated so
     * execution can continue immediately on the next visit.
     */
    AiNodeState visit(AiContext& context) override;

    /**
     * @brief Takes ownership of the given AiNode unique_ptr and adds it to the priority list.
     *
     * Does nothing if the node_ptr is a nullptr.
     */
    void add(i32 priority, std::unique_ptr<AiNode>&& node_ptr) {
        if (!node_ptr.get()) {
            return;
        }
        nodes_.emplace_back(std::move(node_ptr), priority);
        post_create_node();
    }

    /**
     * @brief Constructs and returns a node of the given type in a prioritized list.
     *
     * A higher priority causes the node to be visited before others.
     *
     * Clears the state of all child nodes on each invocation.
     */
    template<typename Node, typename... NodeArgs>
    Node& emplace(i32 priority, NodeArgs&&... args) {
        auto node_ptr = new Node(std::forward<NodeArgs>(args)...);
        node_ptr->parent_ = this;
        auto& node = nodes_.emplace_back(node_ptr, priority);
        post_create_node();
        return *node_ptr;
    }

    /**
     * @brief Clears all child nodes and resets the continuation iterator
     */
    void clear() override;
private:
    struct PrioritizedNode {
        explicit PrioritizedNode(std::unique_ptr<AiNode>&& ptr, i32 priority = 0);
        PrioritizedNode(const PrioritizedNode& other);
        PrioritizedNode(PrioritizedNode&& other);
        PrioritizedNode& operator=(PrioritizedNode other);

        friend void swap(PrioritizedNode& lhs, PrioritizedNode& rhs) {
            using std::swap;
            swap(lhs.priority, rhs.priority);
            swap(lhs.ptr, rhs.ptr);
        }

        std::unique_ptr<AiNode> ptr;
        i32 priority = 0;
        private:
            PrioritizedNode() = default;
    };

    std::vector<PrioritizedNode> nodes_;
    std::vector<PrioritizedNode>::iterator continuation_iterator = nodes_.begin();

    /**
     * @brief Sets the up node and manages priority list after creation of a node
     */
    void post_create_node();
};

#endif
