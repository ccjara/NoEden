#ifndef NOEDEN_AI_SELECTOR_HXX
#define NOEDEN_AI_SELECTOR_HXX

#include "ai/generic_ai_node.hxx"

/**
 * @brief Composite selector node implementation using prioritized nodes
 * 
 * Purpose: Runs child nodes in order of priority.
 * Behavior: Stops executing child nodes when one of them returns `succeeded`.
 * Communication: Returns success if any child node succeeded.
 */
class AiSelector : public GenericAiNode<AiSelector> {
public:
    AiSelector() = default;
    AiSelector(const AiSelector& other);
    AiSelector(AiSelector&& other);
    AiSelector& operator=(AiSelector other);

    friend void swap(AiSelector& lhs, AiSelector& rhs) {
        using std::swap;
        swap(lhs.nodes_, rhs.nodes_);
        lhs.nodes_.swap(rhs.nodes_);
        lhs.iter = lhs.nodes_.begin();
        rhs.iter = rhs.nodes_.begin();
    }

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
    std::vector<PrioritizedNode>::iterator iter = nodes_.begin();

    /**
     * @brief Sets the up node and manages priority list after creation of a node
     */
    void post_create_node();
};

#endif
