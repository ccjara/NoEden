#ifndef JARALYN_DISPLAY_HXX
#define JARALYN_DISPLAY_HXX

#include "../grid.hxx"

struct j_text_state {
    j_color color;
    bool break_word;
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
    /**
     * @brief code point (arbitrary code page) to render from this cell
     */
    int32_t glyph { 0 };

    /**
     * @brief Color to use when rendering this cell
     */
    j_color color = j_color::white();

    j_display_cell() = default;

    j_display_cell(int32_t glyph, j_color color) : glyph(glyph), color(color) {
    }
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
private:
    constexpr static unsigned char CONTROL_CHAR { '$' };
    constexpr static size_t MAX_STATES { 128 };

    std::array<j_text_state, MAX_STATES> states_;

    j_text_state* state_ { states_.data() };
    j_text_state* const first_state_ = { &states_.front() };
    j_text_state* const last_state_ = { &states_.back() };
public:
    j_display();

    /**
     * @brief Render a text to the display
     *
     * If a clamp coordinate is set to the numeric maximum the renderer will
     * constrain the text to the display's width or height respectively.
     * See below for more information about break modes.
     *
     * Text is formattable using the control character $. When encountering it,
     * the text renderer will apply formatting rules based on the desired operation.
     *
     * Depending on the operation, the text renderer may push a new formatting
     * state on its state stack. This way text renderer 'remembers' what format
     * was used when the new formatting is undone.
     *
     * $c renders all subsequent characters in the given color (hex).
     *    usage: $cRRGGBB (RRGGBB as hex)
     * $! pops the current formatting state from the state stack, restoring the
     *    previously active state. if the stack is empty, this will be a noop
     * $n breaks the current line (does not modify the state stack)
     *    usage: Hello $n World
     * $w set text break mode to `break word` (default) which renders clipping
     *    words on the next line (unless the next line is vertically clipping)
     * $a set text break mode to `break all` which immediately renders a clipping
     *    character on the next line, regardless if it is mid-word or not
     */
    void text(
        std::string_view t,
        j_vec2<uint32_t> position,
        j_vec2<uint32_t> clamp = {
            std::numeric_limits<uint32_t>::max(),
            std::numeric_limits<uint32_t>::max()
        }
    );

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
