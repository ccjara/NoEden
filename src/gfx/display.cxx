#include "display.hxx"

const j_display_cell j_display_cell::null = j_display_cell();

void j_display::text(const std::string& t, const j_text_options& options) {
    j_position<uint32_t> position {
        options.boundary.left,
        options.boundary.top
    };
    j_position<uint32_t> limit {
        options.boundary.right,
        options.boundary.bottom
    };
    if (!limit.x || limit.x > dimensions_.width) {
        limit.x = dimensions_.width;
    }
    if (!limit.y || limit.y > dimensions_.height) {
        limit.y = dimensions_.height;
    }
    if (!in_bounds(position) || limit.x < position.x || limit.y < position.y) {
        return;
    }
    const size_t text_length { t.length() };

    const auto needs_break = [&](size_t i) -> bool {
        auto x { position.x };

        while (x <= limit.x) {
            if (i >= text_length) {
                return false;
            }
            switch (t[i]) {
            case ' ':
            case '!':
            case '?':
            case '.':
            case ':':
            case ',':
            case ';':
                return false;
            }
            x++;
            i++;
        }
        return true;
    };

    for (size_t i = 0; i < text_length; i++) {
        if (position.y > limit.y) {
            break;
        }
        bool is_last_char_of_line { position.x == limit.x };
        const char c { t[i] };

        if (options.text_break == j_text_break::break_word) {
            if (c == ' ' && needs_break(i + 1)) {
                is_last_char_of_line = true;
            } else {
                cells_.at(to_index(position)).glyph = c;
            }
        } else {
            cells_.at(to_index(position)).glyph = c;
        }

        if (is_last_char_of_line) {
            position.x = options.boundary.left;
            position.y++;

            if (i + 1 < text_length && t[i + 1] == ' ') {
                // skip immediate space at new line
                i++;
                continue;
            }
        } else {
            position.x++;
        }
    }
}

void j_display::rectangle(const j_rect_options& options) {
    if (!in_bounds({ options.span.right, options.span.bottom })) {
        return;
    }
    for (uint32_t y { options.span.top }; y <= options.span.bottom; y++) {
        const bool t_span { y == options.span.top };
        const bool b_span { y == options.span.bottom };

        for (uint32_t x { options.span.left }; x <= options.span.right; x++) {
            const bool l_span { x == options.span.left };
            const bool r_span { x == options.span.right };
            const auto index { to_index({ x, y }) };

            uint32_t glyph;
            j_rgb<float_t> color = options.color;
                
            if (l_span) {
                if (t_span) {
                    glyph = options.corner_glyphs.top_left;
                } else if (b_span) {
                    glyph = options.corner_glyphs.bottom_left;
                } else {
                    glyph = options.border_glyphs.left;
                }
            } else if (r_span) {
                if (t_span) {
                    glyph = options.corner_glyphs.top_right;
                } else if (b_span) {
                    glyph = options.corner_glyphs.bottom_right;
                } else {
                    glyph = options.border_glyphs.right;
                }
            } else if (t_span) {
                glyph = options.border_glyphs.top;
            } else if (b_span) {
                glyph = options.border_glyphs.bottom;
            } else {
                if (!options.fill_color) {
                    continue;
                }
                glyph = 745U;
                color = options.fill_color.value();
            }
            cells_.at(index).color = color;
            cells_.at(index).glyph = glyph;
        }
    }
}
