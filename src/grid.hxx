#ifndef JARALYN_GRID_HXX
#define JARALYN_GRID_HXX

/**
 * @brief Tag group determining access behavior on the grid
 */
struct grid_access_tag {};

/**
 * @brief Fast access behavior, no bounds checking, invokes UB when accessing invalid locations
 */
struct fast_grid_access_tag : grid_access_tag {};

/**
 * @brief Safe access behavior, includes bounds checking and returns fallbacks for invalid locations
 */
struct safe_grid_access_tag : grid_access_tag {};

/**
 * @brief Generic implementation of a two-dimensional grid
 *
 * Most methods which access specific locations using a templatized `location_type`
 * accept either coordinates packed into a `Vec2<u32>` or an integral typed index
 *
 * Grid is not suitable for very large dimensions, i.e. dimensions which would
 * produce indices larger than u32 could represent. (TODO: assertions)
 */
template<typename cell>
class Grid {
public:
    using cell_type = cell;
    using container_type = std::vector<cell_type>;
protected:
    container_type cells_;
    Vec2<u32> dimensions_;

    /**
     * @brief Fallback during safe-access when accessing an out of bounds location
     */
    const cell null_cell_;

    /**
     * @brief Helper which guarantees an index location return type
     *
     * If the `location_type` is an index, it will be returned as-is
     * otherwise the location is converted from the assumed index location.
     */
    template<typename location_type>
    [[nodiscard]] constexpr size_t ensure_index(location_type location) const {
        if constexpr (std::is_base_of_v<Vec2<u32>, location_type>) {
            return to_index(location);
        } else {
            return location;
        }
    }

    /**
     * @brief Helper which guarantees a coordinate location return type
     *
     * If the `location_type` is a coordinate, it will be returned as-is
     * otherwise the location is converted from the assumed index location.
     */
    template<typename location_type>
    [[nodiscard]] constexpr Vec2<u32> ensure_coordinates(location_type location) const {
        if constexpr (std::is_base_of_v<Vec2<u32>, location_type>) {
            return location;
        } else {
            static_assert(std::is_integral_v<location_type>);
            return to_coordinates(location);
        }
    }
public:
    /**
     * @brief Initializes the grid with the null cell
     *
     * The null cell is returned as an alternative if grid access is out of bounds
     */
    constexpr explicit Grid(cell &&null_cell) : null_cell_(std::move(null_cell)) {
    }

    /**
     * @brief Resets and resizes the grid to the given cell dimensions
     */
    constexpr void resize(const Vec2<u32>& dimensions) {
        cells_.resize(dimensions.x * dimensions.y, null_cell_);
        dimensions_ = dimensions;
        reset();
    }

    /**
     * @brief Fills all cells in the grid with a copy of the null cell
     */
    constexpr void reset() {
        std::fill(cells_.begin(), cells_.end(), null_cell_);
    }

    /**
     * @brief Returns a pointer to the first element of the internal container
     */
    [[nodiscard]] constexpr const cell* data() const {
        return cells_.data();
    }

    /**
     * @brief Returns the amount of cells in the grid
     */
    [[nodiscard]] constexpr size_t cell_count() const {
        return cells_.size();
    }

    /**
     * @brief Returns the total grid size in bytes
     */
    [[nodiscard]] constexpr size_t byte_size() const {
        return cells_.size() * sizeof(cell);
    }

    /**
     * @brief Returns the width and height in of the grid
     */
    [[nodiscard]] constexpr Vec2<u32> dimensions() const {
        return dimensions_;
    }

    /**
     * @brief Returns the width and height in of the grid
     */
    [[nodiscard]] constexpr container_type& cells() {
        return cells_;
    }

    /**
     * @brief Returns a readonly reference to the cell at the given location.
     *
     * The location is range checked by default, but you may override this behavior
     * by providing the `fast_grid_access_tag` which places the burden of bounds
     * verification onto you. `fast_grid_access_tag` in conjunction with an invalid
     * location leads to a dangling reference (UB).
     *
     * If using `safe_access_t` and the location is invalid, a reference to the
     * null-cell is returned instead.
     */
    template<typename location_type>
    [[nodiscard]] constexpr const cell& at(location_type location, grid_access_tag = safe_grid_access_tag {}) const {
        if constexpr (std::is_same_v<grid_access_tag, safe_grid_access_tag>) {
            if (!in_bounds(location)) {
                return null_cell_;
            }
        }
        return cells_.at(ensure_index(location));
    }

    /**
     * @brief Assigns the given cell to the given location
     *
     * If using `safe_access_t` (default) and the position is out of bounds,
     * the operation will be ignored.
     */
    template<typename cel, typename location_type>
    constexpr void put(cel&& c, location_type location, grid_access_tag = safe_grid_access_tag {}) {
        if constexpr (std::is_same_v<grid_access_tag, safe_grid_access_tag>) {
            if (!in_bounds(location)) {
                const auto coord { ensure_coordinates(location) };

                Log::error(
                    "Placement at {}, {}, not within bounds ({}, {})",
                    coord.x, coord.y, dimensions_.x, dimensions_.y
                )
                return;
            }
        }
        cells_[ensure_index(location)] = std::forward<cel>(c);
    }

    /**
     * @brief Returns whether the given location is contained by the grid
     *
     * You may either pass a Vec2<u32> to indicate a cartesian coordinate
     * or an index, in which case `location_type` must be integral.
     */
    template<typename location_type>
    [[nodiscard]] constexpr bool in_bounds(location_type location) const {
        if constexpr (std::is_base_of_v<Vec2<u32>, location_type>) {
            return location.x + 1 <= dimensions_.x && location.y + 1 <= dimensions_.y;
        } else {
            static_assert(std::is_integral_v<location_type>);
            return location < cells_.size();
        }
    }

    /**
     * @brief Clamps (mutates) the given coordinates to the grid dimensions
     *
     * This ensures that the coordinates are contained by the grid
     */
    constexpr void clamp(Vec2<u32>& coord) const {
        if (coord.x > dimensions_.x) {
            coord.x = dimensions_.x;
        }
        if (coord.y > dimensions_.y) {
            coord.y = dimensions_.y;
        }
    }

    /**
     * @brief Converts the given index to grid coordinates
     *
     * Using the `fast_grid_access_tag` (default), does not ensure that the grid is
     * non-empty. If using fast access and the grid is empty, the game will
     * panic due to division by zero. When using the `safe_access_t` tag this
     * method will return origin coordinates if the grid's width is zero.
     *
     * This does *not* verify the resulting coordinates.
     */
    [[nodiscard]] constexpr Vec2<u32> to_coordinates(size_t index, grid_access_tag = fast_grid_access_tag {}) const {
        if constexpr (std::is_same_v<grid_access_tag, safe_grid_access_tag>) {
            if (!dimensions_.x) {
                return { 0, 0 };
            }
        }
        return Vec2<u32> {
            static_cast<u32> (index) % dimensions_.x,
            static_cast<u32> (index) / dimensions_.x
        };
    }

    /**
     * @brief Converts the given coordinates to an index
     *
     * This method does *not* verify the resulting index.
     */
    [[nodiscard]] constexpr size_t to_index(Vec2<u32> coord) const {
        return coord.y * dimensions_.x + coord.x;
    }
};

#endif
