#ifndef JARALYN_UI_TREE_HXX
#define JARALYN_UI_TREE_HXX

#include "ui_window.hxx"

class j_ui_tree {
private:
    /**
     * @brief A key value store mapping id to node
     */
    std::unordered_map<std::string, std::unique_ptr<j_ui_node>> nodes_;

    /**
     * @brief The head of the tree, must not be nullptr after construction
     */
    j_ui_node* root_;
public:
    /**
     * @brief Constructs the root node
     */
    j_ui_tree() {
        reset();
    }

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
    template<typename node>
    node* create_node(j_ui_node* parent, std::string_view id) {
        if (parent == nullptr) {
            parent = root_;
        }
        auto [iterator, inserted] = nodes_.try_emplace(
            std::string(id),
            std::make_unique<node>()
        );
        if (!inserted) {
            LOG(ERROR) << "Cannot create node " << id << ": a node with this id already exists";
            return nullptr;
        }
        auto n = iterator->second.get();
        n->id_ = id;
        n->parent_ = parent;
        n->anchor_.node = root_;
        parent->children_.push_back(n);
        return static_cast<node*>(n);
    }

    j_ui_node* get_node_by_id(std::string_view id) {
        auto node_iter { nodes_.find(std::string(id)) };
        if (node_iter == nodes_.end()) {
            return nullptr;
        }
        return node_iter->second.get();
    }

    /**
     * @brief Returns the head of the tree, guaranteed to be non-null
     */
    j_ui_node* root() const {
        return root_;
    }

    void reset() {
        nodes_.clear();
        auto [iterator, inserted] = nodes_.try_emplace(
            std::string("root"),
            std::make_unique<j_ui_node>()
        );
        assert(inserted);

        root_ = iterator->second.get();
        root_->id_ = iterator->first;
    }
};

#endif
