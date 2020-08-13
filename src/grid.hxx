#ifndef JARALYN_GRID_HXX
#define JARALYN_GRID_HXX

template<typename cell>
class j_grid {
protected:
    std::vector<cell> cells_;
    j_size<uint32_t> dimensions_ { 0, 0 };

    template<typename pos>
    [[nodiscard]] constexpr inline auto to_index(const pos& position) const noexcept {
        return position.y * dimensions_.width + position.x;
    }
public:
    using cell_type = cell;

    j_grid() = default;

    template<typename sz>
    constexpr explicit j_grid(const sz& size) {
        resize(size);
    }

    /**
     * @brief Resets and resizes the grid to the given cell dimensions
     */
    template<typename sz>
    constexpr void resize(const sz& dimensions) {
        cells_.resize(dimensions.area(), cell::null);
        dimensions_ = dimensions;
        reset();
    }

    constexpr void reset() noexcept {
        std::fill(cells_.begin(), cells_.end(), cell::null);
    }

    /**
     * @brief Provides iterable access to all cells of the grid
     */
    constexpr const auto* data() const noexcept {
        return cells_.data();
    }

    /**
     * @brief Returns the amount of cells in the grid
     */
    constexpr auto size() const noexcept {
        return cells_.size();
    }

    /**
     * @brief Returns the total grid size in bytes
     */
    constexpr auto byte_size() const noexcept {
        return size() * sizeof(cell);
    }

    constexpr auto dimensions() const noexcept {
        return dimensions_;
    }

    /**
     * @brief Returns the cell at the given zero-starting position
     *
     * If the position is out of bounds, a null cell is returned.
     */
    template<typename pos>
    [[nodiscard]] constexpr inline const cell& at(const pos& position) const noexcept {
        if (!in_bounds(position)) {
            return cell::null;
        }
        return cells_.at(to_index(position));
    }

    /**
     * @brief Moves the given cell to the given position
     *
     * If the position is out of bounds, the operation will be ignored.
     */
    template<typename pos>
    constexpr void put(cell&& c, const pos& position) {
        if (!in_bounds(position)) {
            LOG(ERROR)
                << "Placement at " << position.x << ", " << position.y
                << " not within bounds ("
                << dimensions_.width << ", " << dimensions_.height << ")";
            return;
        }
        cells_[to_index(position)] = std::move(c);
    }

    /**
     * @brief Returns whether the given position is within bounds
     */
    template<typename pos>
    [[nodiscard]] constexpr inline bool in_bounds(const pos& position) const noexcept {
        return position.x + 1 <= dimensions_.width && position.y + 1 <= dimensions_.height;
    }

    /**
     * @brief Clamps the given position to the grid dimensions
     *
     * This ensures that the position is contained by the grid
     */
    template<typename pos>
    constexpr inline void clamp(pos& position) noexcept {
        if (position.x > dimensions_.width) {
            position.x = dimensions_.width;
        }
        if (position.y > dimensions_.height) {
            position.y = dimensions_.height;
        }
    }
};

#endif
