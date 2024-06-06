#ifndef NOEDEN_UI_TREE_HXX
#define NOEDEN_UI_TREE_HXX

#include "./ui_node.hxx"

class UiTree {
    using NodeContainer = std::vector<std::unique_ptr<UiNode>>;
public:
    /**
     * @brief Creates and tracks a node
     *
     * The node will become a child of the optionally given parent node. If
     * no parent is specified, i.e. nullptr is given, the root node will be
     * automatically assumed. The only node without a parent is the root node.
     *
     * The given id must be unique, otherwise the creation will fail and nullptr
     * will be returned instead.
     */
    template<typename Node>
    Node* create_node(UiNode* parent, std::string_view id) {
        assert(root_); // root node is created in a dedicate method
        if (parent == nullptr) {
            parent = root_;
        }

        if (nodes_by_id_.find(std::string(id)) != nodes_by_id_.end()) {
            Log::error("Cannot create node #{}: a node with this id already exists");
            return nullptr;
        }

        auto node = std::make_unique<Node>();
        auto node_raw_ptr = static_cast<Node*> (node.get());
        nodes_by_id_.emplace(std::string(id), node_raw_ptr);
        node->root_ = root_;
        node->id_ = id;
        node->parent_ = parent;
        node->anchor_to(root_);
        parent->children_.push_back(node_raw_ptr);
        nodes_.emplace_back(std::move(node));
        return node_raw_ptr;
    }

    /**
     * @brief Removes a node by the given id
     *
     * Does nothing if no node having the given id exists.
     */
    void remove_node(std::string_view id);

    UiNode* get_node_by_id(std::string_view id);

    /**
     * @brief Returns the head of the tree, guaranteed to be non-null
     */
    UiNode* root() const;

    /**
     * @brief Resets the entire tree and its references and recreates the root node.
     */
    void reset();

    /**
     * @brief Resets the entire tree and its references.
     *
     * Used during shutdown.
     */
    void clear();

    NodeContainer& nodes();

    /**
     * @brief Create the root node.
     *
     * Should only be called if the root node does not exist yet.
     */
    void create_root_node();
private:
    /**
     * @brief Primary owning container for each node
     */
    NodeContainer nodes_;

    /**
     * @brief A key value store mapping id to node
     */
    std::unordered_map<std::string, UiNode*> nodes_by_id_;

    /**
     * @brief The head of the tree
     */
    UiNode* root_ { nullptr };

    /**
     * @brief Recursively marks the target node and its children as destroyed
     */
    void destroy_node(UiNode* node);
};

#endif
