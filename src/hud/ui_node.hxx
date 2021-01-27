#ifndef JARALYN_UI_NODE_HXX
#define JARALYN_UI_NODE_HXX

/**
 * @brief Affects position of elements, allowing relative positioning
 */
enum class j_ui_anchor_origin {
    top,
    top_right,
    right,
    bottom_right,
    bottom,
    bottom_left,
    left,
    top_left,
    center,
};

// TODO: port stuff from ui_tree.hxx to ui_node.cxx, add implementations

/**
 * @brief RTTI for ui nodes
 */
enum class j_ui_node_type {
    generic_wrapper,
    window,
};

class j_ui_node {
    friend class j_ui_tree;
public:
    /**
     * @brief Encapsulates information about this node's anchoring
     */
    struct j_ui_anchor {
        /**
         * @brief A destination node this node is anchored to or nullptr if not anchored
         * 
         * If a node is removed, all anchor node pointers are nulled.
         */
        j_ui_node* node { nullptr };

        /**
         * @brief A list of nodes anchored to this node
         */
        std::vector<j_ui_node*> nodes;

        /**
         * @brief The anchor origin used to calculate an absolute position
         */
        j_ui_anchor_origin origin = j_ui_anchor_origin::top_left;
    };

    /**
     * @brief Readonly accessor for the id_ attribute
     * 
     * Written to by the factory after construction
     */
    std::string_view id() const;

    /**
     * @brief Readonly accessor for the type_ attribute
     * 
     * Written to by the factory after construction
     */
    j_ui_node_type type() const;

    /**
     * @brief Sets the current, relative (untranslated) position of this node
     * 
     * Its absolute position will be calculated based on its anchor.
     * 
     * Also recursivly moves all anchored nodes based on their anchor settings.
     */
    void move(j_vec2<uint32_t> position);

    /**
     * @brief Writing accessor for the size_ attribute
     */
    void resize(j_vec2<uint32_t> size);

    /**
     * @brief Returns the position relative to its anchor
     * 
     * This position may be modified using move()
     * 
     * @see move()
     */
    j_vec2<uint32_t> relative_position() const;

    /**
     * @brief Returns the translated position (as it will appear on screen)
     * 
     * This value depends on the relative position and anchoring and is updated
     * whenever the move() function adjusts the relative position of a node.
     * 
     * @see move()
     */
    j_vec2<uint32_t> absolute_position() const;

    /**
     * @brief Readonly accessor for the anchor_ attribute
     */
    const j_ui_anchor& anchor() const;

    /**
     * @brief Updates the anchor origin and recalculates its absolute position
     */
    void set_anchor_origin(j_ui_anchor_origin origin);

    /**
     * @brief Readonly accessor for the children_ attribute
     */
    const std::vector<j_ui_node*>& children() const;

    /**
     * @brief Readonly accessor for the size_ attribute
     */
    j_vec2<uint32_t> size() const;
protected:
    /**
     * @brief Unique id referencing this node
     * 
     * The pointer is owned by the tree
     * 
     * @see j_ui_tree::nodes_
     */
    std::string_view id_;

    /**
     * @brief RTTI
     */
    j_ui_node_type type_ = j_ui_node_type::generic_wrapper;

    /**
     * @brief Anchor settings for this node
     */
    j_ui_anchor anchor_;

    /**
     * @brief Logical grouping of several nodes, does not affect positioning, but existence.
     * 
     * If a parent container is removed, all child containers are also removed.
     */
    std::vector<j_ui_node*> children_;

    /**
     * @brief Containing parent node of this node, always set unless root
     */
    j_ui_node* parent_ { nullptr };

    /**
     * @brief Relative position as specified by the ui code
     * 
     * An absolute position will be calculated using anchor information.
     * 
     * @see absolute_position
     * @see anchor
     */
    j_vec2<uint32_t> relative_position_;

    /**
     * @brief Absolute position, used by the renderer
     *
     * The absolute position should not be written to directly by ui code.
     * The j_ui_tree implementation is responsible to arrange all nodes
     * based on their relative position and anchor.
     * 
     * @see relative_position
     */
    j_vec2<uint32_t> absolute_position_;

    /**
     * @brief Size of this node, affects positioning of anchored nodes
     */
    j_vec2<uint32_t> size_;

private:
    /**
     * @brief Assigns an absolute position to this node
     * 
     * If the node has anchored nodes, these will be repositioned
     * subsequently, repeating the process recursively for all anchors
     * until no anchors are left.
     * 
     * Note that this method assigns the position as-is so the given absolute
     * position must be verified.
     */
    void move_abs(j_vec2<uint32_t> pos);
};

j_vec2<uint32_t> calc_anchor_offset(
    const j_ui_node& target,
    const j_ui_node& parent
);

#endif
