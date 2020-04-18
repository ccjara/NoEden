#ifndef JARALYN_DISPLAY_HXX
#define JARALYN_DISPLAY_HXX

#include "../grid.hxx"

enum class j_text_break {
    break_word,
    break_all,
};

struct j_text_options {
    j_rect<uint32_t> boundary;
    j_color color;
    j_text_break text_break = j_text_break::break_word;
};

struct j_rect_options {
    j_rect<uint32_t> span;
    j_color color;
    std::optional<j_color> fill_color { std::nullopt };
    j_rect<uint32_t> border_glyphs { 745U, 745U, 745U, 745U };
    j_rect_corners<uint32_t> corner_glyphs {
        745U,
        745U,
        745U,
        745U
    };
};

struct j_display_cell {
public:
    int32_t glyph { 0 };
    j_color color;

    static const j_display_cell null;
};

class j_display: public j_grid<j_display_cell> {
public:
    void put(j_display_cell&& cell, j_vec2<uint32_t> pos);
    void text(const std::string& t, const j_text_options& options);
    void rectangle(const j_rect_options& options);
    void line(j_vec2<uint32_t> from, j_vec2<uint32_t> to, uint32_t glyph = 750, j_color color = j_color());
};

#endif
