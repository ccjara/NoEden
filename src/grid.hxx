#ifndef JARALYN_GRID_HXX
#define JARALYN_GRID_HXX

template<typename cell_t>
class j_grid {
protected:
    std::vector<cell_t> cells_;
    j_size<uint32_t> dimensions_ { 0, 0 };

    uint32_t to_index(j_position<uint32_t> pos) const noexcept {
        return pos.y * dimensions_.width + pos.x;
    }
public:
    using cell_type = cell_t;

    j_grid(std::optional<j_size<uint32_t>> size = std::nullopt) {
        if (size.has_value()) {
            resize(size.value());
        }
    }

    /**
     * @brief Resizes the grid to the given dimensions
     *
     * As of now this also resets the entire grid
     */
    void resize(j_size<uint32_t> dimensions) {
        cells_.resize(dimensions.area(), cell_t::null);
        dimensions_ = dimensions;
        reset();
    }

    void reset() {
        std::fill(cells_.begin(), cells_.end(), cell_t::null);
    }

    /**
     * @brief Provides iterable access to all cells of the grid
     */
    const cell_t* data() const noexcept {
        return cells_.data();
    }

    /**
     * @brief Returns the amount of cells in the grid
     */
    typename std::vector<cell_t>::size_type size() const noexcept {
        return cells_.size();
    }

    /**
     * @brief Returns the total grid size in bytes
     */
    size_t byte_size() const noexcept {
        return size() * sizeof(cell_t);
    }

    j_size<uint32_t> dimensions() const noexcept {
        return dimensions_;
    }

    /**
     * @brief Returns the cell at the given zero-starting position
     *
     * If the position is out of bounds, a null cell is returned.
     */
    const cell_t& at(j_position<uint32_t> pos) const noexcept {
        if (!in_bounds(pos)) {
            return cell_t::null;
        }
        return cells_.at(to_index(pos));
    }

    /**
     * @brief Moves the given cell to the given position
     *
     * If the position is out of bounds, the operation will be ignored.
     */
    void set(j_position<uint32_t> pos, cell_t&& cell) {
        if (!in_bounds(pos)) {
            LOG(ERROR)
                << "Placement at " << pos.x << ", " << pos.y
                << " not within bounds ("
                << dimensions_.width << ", " << dimensions_.height << ")";
            return;
        }
        cells_[to_index(pos)] = std::move(cell);
    }

    /**
     * @brief Returns whether the given position is within bounds
     */
    bool in_bounds(j_position<uint32_t> pos) const noexcept {
        return pos.x + 1 <= dimensions_.width && pos.y + 1 <= dimensions_.height;
    }
};

#endif
