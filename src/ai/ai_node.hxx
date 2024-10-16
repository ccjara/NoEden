#pragma once

#include "ai/ai_context.hxx"

enum class AiNodeState {
    Ready,
    Running,
    Failed,
    Succeeded,
};

enum class AiNodeType { // TODO integrate in C++ classes
    Invalid = 0,
    Sequence = 1,
    Selector = 2,
    Condition = 3,
    ClosestEntity = 4,

    Walk = 1000,
};

/**
 * @brief Base class for every behavior tree node, abbrev. as 'Ai'
 *
 * The current implementation is probably a cache trasher and has lots of
 * indirections, but it will do fine for now.
 */
class AiNode {
public:
    AiNode* parent_ = nullptr;

    /**
     * @brief Initializes this node with the given context
     */
    virtual void initialize(AiContext& context);

    virtual ~AiNode() = default;

    /**
     * @brief Visits and runs a node. The behavior is implementation defined.
     */
    virtual AiNodeState visit(AiContext& context) = 0;

    /**
     * @brief Implementation defined way of clearing its state
     */
    virtual void clear() = 0;

    /**
     * @brief Clones a heap-allocated AiNode
     */
    [[nodiscard]] virtual std::unique_ptr<AiNode> clone() const = 0;

    AiNodeState state() const;

    bool failed() const;
protected:
    AiNode() = default;

    AiNodeState mod_state(AiNodeState s);
private:
    AiNodeState state_ = AiNodeState::Ready; // use mod_state() to modify
};

/**
 * \brief Parses an AiNodeType from a string
 *
 * \returns A parsed AiNodeType != `Invalid` on success, `Invalid` otherwise.
 */
[[nodiscard]] AiNodeType parse_node_type(std::string_view type);
