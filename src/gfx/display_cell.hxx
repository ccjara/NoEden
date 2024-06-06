#ifndef NOEDEN_DISPLAY_CELL_HXX
#define NOEDEN_DISPLAY_CELL_HXX

/**
 * @brief A single element of a display.
 *
 * @see Display
 */
struct DisplayCell {
    /**
     * @brief code point (arbitrary code page) to render from this cell
     */
    i32 glyph { 0 };

    /**
     * @brief Color to use when rendering this cell
     */
    Color color = Color::white();

    DisplayCell() = default;

    DisplayCell(i32 glyph, Color color) : glyph(glyph), color(color) {
    }
};

#endif