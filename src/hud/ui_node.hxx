#ifndef JARALYN_UI_NODE_HXX
#define JARALYN_UI_NODE_HXX

/**
 * @brief Affects position of elements, allowing relative positioning
 */
enum class j_ui_anchor_origin : uint32_t {
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
     * @brief Readonly accessor for the id_ attribute
     *
     * Written to by the factory after construction
     */
    const std::string& id() const;

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
     * @brief Anchors this node to the given node
     *
     * Existing anchor references will be updated accordingly.
     */
    void anchor_to(j_ui_node& node);

    /**
     * @brief Updates the anchor origin and recalculates its absolute position
     */
    void set_anchor_origin(j_ui_anchor_origin origin);

    /**
     * @brief Returns the node this node is currently anchored to
     */
    j_ui_node* anchored_to() const;

    /**
     * @brief Readonly accessor for the anchored_by_ attribute
     */
    const std::vector<j_ui_node*>& anchored_by() const;

    /**
     * @brief Readonly accessor for the origin_ attribute
     */
    j_ui_anchor_origin anchor_origin() const;

    /**
     * @brief Readonly accessor for the children_ attribute
     */
    const std::vector<j_ui_node*>& children() const;

    /**
     * @brief Returns true if this node can be anchored to the given one
     */
    bool can_anchor_to(j_ui_node* node) const;

    /**
     * @brief Readonly accessor for the size_ attribute
     */
    j_vec2<uint32_t> size() const;

    /**
     * @brief Returns true if the node is the root node
     */
    bool is_root() const;

    /**
     * @brief Readonly accessor for the visible_ attribute
     */
    bool visible() const;

    /**
     * @brief Sets visibility to true
     */
    void show();

    /**
     * @brief Sets visibility to false
     */
    void hide();
protected:
    /**
     * @brief A list of nodes anchored to this node
     *
     * When accessing this list, check for nullptrs as the list is not
     * defragmented on every frame.
     */
    std::vector<j_ui_node*> anchored_by_;

    /**
     * @brief A destination node this node is anchored to
     *
     * If the destination node is removed and this node is not a child node
     * thereof this node will be re-anchored to the root node.
     */
    j_ui_node* anchored_to_ { nullptr };

    /**
     * @brief Origin used to calculate an absolute position
     */
    j_ui_anchor_origin anchor_origin_ = j_ui_anchor_origin::top_left;

    /**
     * @brief Unique id referencing this node
     */
    std::string id_;

    /**
     * @brief RTTI
     */
    j_ui_node_type type_ = j_ui_node_type::generic_wrapper;

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

    /**
     * @brief Visibility behavior is node specific
     */
    bool visible_ = false;

    /**
     * @brief Updates the position of this node's anchors
     */
    void move_anchors();
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
