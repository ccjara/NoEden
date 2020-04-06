#ifndef JARALYN_GRID_HXX
#define JARALYN_GRID_HXX

#include "cell.hxx"

class grid {
private:
    std::vector<cell> data_;
    size<uint32_t> dimensions_ { 0, 0 };

    cell null_cell_ = { 0 };

    uint32_t to_index(position<uint32_t> pos) const noexcept;
public:
    /**
     * @brief Resizes the grid to the given dimensions
     * 
     * As of now this also resets the entire grid
     */
    void resize(size<uint32_t> dimensions);

    /**
     * @brief Returns the cell at the given zero-starting position
     *
     * If the position is out of bounds, a null cell is returned.
     */
    const cell& at(position<uint32_t> pos) const noexcept;

    /**
     * @brief Assigns the given cell value to the given position
     *
     * If the position is out of bounds, the operation will be ignored.
     */
    void set(position<uint32_t> pos, int value);

    /**
     * @brief Returns whether the given position is within bounds
     */
    bool in_bounds(position<uint32_t> pos) const noexcept;

    void prnt();
};

#endif
