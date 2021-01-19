#ifndef JARALYN_UI_TREE_HXX
#define JARALYN_UI_TREE_HXX

enum class j_ui_node_type {
    generic_wrapper,
    window,
};

struct j_ui_node {
    j_ui_node_type node_type;
    std::string_view id;
    std::vector<j_ui_node*> children;
    j_ui_node* parent { nullptr };

    j_vec2<uint32_t> position;
    j_vec2<uint32_t> size;
};

struct j_ui_window : public j_ui_node {
    std::string title;
};

class j_ui_tree {
private:
    std::unordered_map<std::string, std::unique_ptr<j_ui_node>> nodes_;

    j_ui_node* root_;

    template<typename node>
    node* create_node(j_ui_node* parent, std::string_view id) {
        assert(parent);

        auto [iterator, inserted] = nodes_.try_emplace(std::string(id), std::make_unique<node>());
        if (!inserted) {
            LOG(ERROR) << "Cannot create node " << id << ": a node with this id already exists";
            return nullptr;
        }
        auto n = iterator->second.get();
        n->id = id;
        n->parent = parent;
        parent->children.push_back(n);
        return static_cast<node*>(n);
    }
public:
    j_ui_tree() {
        auto [iterator, inserted] = nodes_.try_emplace(std::string("root"), std::make_unique<j_ui_node>());
        assert(inserted);

        root_ = iterator->second.get();
        root_->id = iterator->first;
    }

    j_ui_node* root() const {
        return root_;
    }

    j_ui_window* create_window(j_ui_node* parent, std::string_view id) {
        auto window { create_node<j_ui_window>(parent, id) };
        window->node_type = j_ui_node_type::window;
        return window;
    }
};

#endif