#ifndef JARALYN_DISPLAY_HXX
#define JARALYN_DISPLAY_HXX

#include "../grid.hxx"

enum class j_text_break {
    break_all, // break immediately when clipping
    break_word, // go back and break the entire word
};

struct j_text_options {
    /**
     * @brief Constrains the text to a boundary (viewed in positive direction)
     *
     * Horizontally clipping text will be rendered on the next line,
     * depending on the {@see text_break} option.
     *
     * Vertically clipping text will not be rendered.
     *
     * If a coordinate is set to the numeric maximum the renderer will constrain
     * it to the dimensions of the display, effectively meaning 'clamp to display'
     */
    j_vec2<uint32_t> clamp {
        std::numeric_limits<uint32_t>::max(),
        std::numeric_limits<uint32_t>::max()
    };

    /**
     * @brief Color to use the render the text
     */
    j_color color;

    /**
     * @brief Text break on clipping text, considering the {@see boundary}
     */
    j_text_break text_break = j_text_break::break_word;
};

struct j_rect_options {
    /**
     * @brief Rectangle span (coordinates)
     */
    j_rect<uint32_t> span;

    /**
     * @brief Border color to use
     */
    j_color color;

    /**
     * @brief Optional fill color to use (transparent, if omitted)
     */
    std::optional<j_color> fill_color { std::nullopt };

    /**
     * @brief Glyphs to use when rendering its sides
     */
    j_rect<uint32_t> border_glyphs { 745U, 745U, 745U, 745U };

    /**
     * @brief Glyphs to use when rendering its corners
     */
    j_rect_corners<uint32_t> corner_glyphs {
        745U,
        745U,
        745U,
        745U
    };
};

/**
 * @brief A single element of a display.
 *
 * @see j_display
 */
struct j_display_cell {
private:
    friend class j_grid<j_display_cell>;

    /**
     * @brief Fallback (null) cell
     */
    static const j_display_cell null;
public:
    /**
     * @brief code point (arbitrary code page) to render from this cell
     */
    int32_t glyph { 0 };

    /**
     * @brief Color to use when rendering this cell
     */
    j_color color;
};

/**
 * @brief A grid comprised of cells representing a single character
 *
 * On each frame the current display state is copied into the text shader
 * which handles the rendering of each individual character.
 *
 * @see j_text_shader
 */
class j_display: public j_grid<j_display_cell> {
public:
    /**
     * @brief Override a single cell of the display
     */
    void put(j_display_cell&& cell, j_vec2<uint32_t> pos);

    /**
     * @brief Render a text on the display
     */
    void text(std::string_view t, j_vec2<uint32_t> position, const j_text_options& options);

    /**
     * @brief Render a rectangle on the display
     */
    void rectangle(const j_rect_options& options);

    /**
     * @brief Render a line on the display using bresenham's algorithm
     */
    void line(j_vec2<uint32_t> from, j_vec2<uint32_t> to, uint32_t glyph = 750, j_color color = j_color());
};

#endif
