#ifndef NOEDEN_DISPLAY_INFO_HXX
#define NOEDEN_DISPLAY_INFO_HXX

/**
 * @brief Display drawing instructions
 */
struct DisplayInfo {
    /**
     * @brief Glyph to render
     */
    i32 glyph = 0U;

    /**
     * @brief Color to use when rendering the glyph
     */
    Color color = Color::black();

    /**
     * @brief Skips drawing this glyph if false
     */
    bool visible = false;
};

#endif
