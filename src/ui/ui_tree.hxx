#ifndef JARALYN_UI_TREE_HXX
#define JARALYN_UI_TREE_HXX

#include "ui_window.hxx"

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
        if (parent == nullptr) {
            parent = root_;
        }

        if (nodes_by_id_.find(std::string(id)) != nodes_by_id_.end()) {
            LOG(ERROR) << "Cannot create node " << id << ": a node with this id already exists";
            return nullptr;
        }

        auto node { new Node() };
        nodes_.emplace_back(node);
        nodes_by_id_.emplace(std::string(id), node);
        node->id_ = id;
        node->parent_ = parent;
        node->anchor_to(*root_);
        parent->children_.push_back(node);
        return node;
    }

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
};

#endif
