#ifndef JARALYN_FOV_HXX
#define JARALYN_FOV_HXX

#include "../grid.hxx"

struct octant {
    char xx;
    char xy;
    char yx;
    char yy;
};

constexpr std::array<octant, 8> octants {
    octant{ 1,  0,  0,  1 },   // 0 E-NE
    octant{ 0,  1,  1,  0 },   // 1 NE-N
    octant{ 0, -1,  1,  0 },   // 2 N-NW
    octant{ -1,  0,  0,  1 },  // 3 NW-W
    octant{ -1,  0,  0, -1 },  // 4 W-SW
    octant{ 0, -1, -1,  0 },   // 5 SW-S
    octant{ 0,  1, -1,  0 },   // 6 S-SE
    octant{ 1,  0,  0, -1 },   // 7 SE-E
};


enum visibility {
    hidden = 0,
    visible = 1,
    wall = 2,
    debug = 3,
};

class j_fov {
private:
    j_grid<int> grid_;
    j_vec2<uint32_t> origin_;
    float_t radius_ { 1.0f };
    float_t radius_sq_ { 1.0f };
    uint32_t radius_ceiling_ { 1 };

    void cast_light(int start_col, float_t leftViewSlope, float_t rightViewSlope, octant oct) {
        // Set true if the previous cell we encountered was blocked.
        bool prevWasBlocked = false;

        // As an optimization, when scanning past a block we keep track of the
        // rightmost corner (bottom-right) of the last one seen.  If the next cell
        // is empty, we can use this instead of having to compute the top-right corner
        // of the empty cell.
        float_t savedRightSlope = -1;

        const int xDim = grid_.dimensions().x;
        const int yDim = grid_.dimensions().y;

        // Outer loop: walk across each column, stopping when we reach the visibility limit.
        for (int currentCol = start_col; currentCol <= radius_ceiling_; currentCol++) {
            int xc = currentCol;

            // Inner loop: walk down the current column.  We start at the top, where X==Y.
            for (int yc = currentCol; yc >= 0; yc--) {
                // Translate local coordinates to grid coordinates.  For the various octants
                // we need to invert one or both values, or swap X for Y.
                int gridX = origin_.x + xc * oct.xx + yc * oct.xy;
                int gridY = origin_.y + xc * oct.yx + yc * oct.yy;

                // Range-check the values.  This lets us avoid the slope division for blocks
                // that are outside the grid.
                //
                // Note that, while we will stop at a solid column of blocks, we do always
                // start at the top of the column, which may be outside the grid if we're (say)
                // checking the first octant while positioned at the north edge of the map.
                if (gridX < 0 || gridX >= xDim || gridY < 0 || gridY >= yDim) {
                    continue;
                }

                // Compute slopes to corners of current block.  We use the top-left and
                // bottom-right corners.
                // Note these values will be outside the view angles for the blocks at the
                // ends -- left value > 1, right value < 0.
                float_t leftBlockSlope = (yc + 0.5f) / (xc - 0.5f);
                float_t rightBlockSlope = (yc - 0.5f) / (xc + 0.5f);

                // Check to see if the block is outside our view area.  Note that we allow
                // a "corner hit" to make the block visible.  Changing the tests to >= / <=
                // will reduce the number of cells visible through a corner (from a 3-wide
                // swath to a single diagonal line), and affect how far you can see past a block
                // as you approach it.  This is mostly a matter of personal preference.
                if (rightBlockSlope > leftViewSlope) {
                    // Block is above the left edge of our view area; skip.
                    continue;
                } else if (leftBlockSlope < rightViewSlope) {
                    // Block is below the right edge of our view area; we're done.
                    break;
                }
                float_t distanceSquared = xc * xc + yc * yc;
                const j_vec2<uint32_t> pos { (unsigned int) gridX, (unsigned int) gridY };

                bool curBlocked = grid_.at(pos) == visibility::wall;

                if (distanceSquared <= radius_sq_) {
                    grid_.put(visibility::visible, pos);
                }

                if (prevWasBlocked) {
                    if (curBlocked) {
                        // Still traversing a column of walls.
                        savedRightSlope = rightBlockSlope;
                    } else {
                        // Found the end of the column of walls.  Set the left edge of our
                        // view area to the right corner of the last wall we saw.
                        prevWasBlocked = false;
                        leftViewSlope = savedRightSlope;
                    }
                } else {
                    if (curBlocked) {
                        // Found a wall.  Split the view area, recursively pursuing the
                        // part to the left.  The leftmost corner of the wall we just found
                        // becomes the right boundary of the view area.
                        //
                        // If this is the first block in the column, the slope of the top-left
                        // corner will be greater than the initial view slope (1.0).  Handle
                        // that here.
                        if (leftBlockSlope <= leftViewSlope) {
                            cast_light(currentCol + 1, leftViewSlope, leftBlockSlope, oct);
                        }

                        // Once that's done, we keep searching to the right (down the column),
                        // looking for another opening.
                        prevWasBlocked = true;
                        savedRightSlope = rightBlockSlope;
                    }
                }
            }

            // Open areas are handled recursively, with the function continuing to search to
            // the right (down the column).  If we reach the bottom of the column without
            // finding an open cell, then the area defined by our view area is completely
            // obstructed, and we can stop working.
            if (prevWasBlocked) {
                break;
            }
        }
    }
public:
    j_fov() : grid_(visibility::hidden) {
    }

    void do_fov(j_vec2<uint32_t> origin, float_t radius) {
        origin_ = origin;
        radius_ = radius;
        radius_ceiling_ = std::ceil(radius_);
        radius_sq_ = radius * radius;

        grid_.put(visibility::visible, origin);

        for (auto oct : octants) {
            cast_light(1, 1.0f, 0.0f, oct);
        }
    }

    j_grid<int>& grid() {
        return grid_;
    }
};

#endif
