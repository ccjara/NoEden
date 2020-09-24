#include "display.hxx"

j_display::j_display() : j_grid<j_display_cell>(j_display_cell{}) {
    state_[0] = j_text_state { j_color::white() };
}

void j_display::text(std::string_view t, j_vec2<uint32_t> position, j_vec2<uint32_t> clamp) {
    current_state_ = 0; // reset state stack
    const j_vec2<uint32_t> limit {
        std::min(clamp.x, dimensions_.x),
        std::min(clamp.y, dimensions_.y)
    };
    if (!in_bounds(position) || limit.x < position.x || limit.y < position.y) {
        return;
    }
    bool break_word { true };
    const auto origin { position };
    const size_t text_length { t.length() };

    const auto needs_break = [&](size_t i) -> bool {
        auto x { position.x };

        while (x <= limit.x) {
            if (i >= text_length) {
                return false;
            }
            switch (t[i]) {
            case CONTROL_CHAR:
                switch (t[i + 1]) {
                    case 'c':
                        i = std::min(text_length, i + 8);
                        continue;
                    case '!':
                    case 'w':
                    case 'a':
                        i = std::min(text_length, i + 2);
                        continue;
                    case 'n':
                        i = std::min(text_length, i + 2);
                        return false;
                    case '\0':
                    default:
                        return false;
                }
                continue;
            case ' ':
                return false;
            }
            x++;
            i++;
        }
        return true;
    };

    j_color current_color = j_color::white();

    for (size_t i { 0 }; i < text_length; ++i) {
        bool is_last_char_of_line { position.x == limit.x };

        if (t[i] == CONTROL_CHAR) {
            switch (t[i + 1]) {
                case 'c': {
                    //      n: 1234567
                    // t[i+n]: cRRGGBB
                    if (i + 7 >= text_length || current_state_ >= MAX_STATES)
                        break;
                    int color_hex { 0xFFFFFF };
                    // note: from_chars processes the interval [first,last)
                    std::from_chars(&t[i + 2], &t[i + 8], color_hex, 16);
                    current_color = color_hex;

                    state_[current_state_ + 1] = state_[current_state_];
                    ++current_state_;
                    state_[current_state_].color = current_color;
                    i += 8;
                    break;
                }
                case 'w':
                case 'a':
                    if (current_state_ < MAX_STATES) {
                        state_[current_state_ + 1] = state_[current_state_];
                        ++current_state_;
                        state_[current_state_].break_word = t[i + 1] == 'w';
                        i += 1;
                        continue;
                    }
                    break;
                case '!':
                    if (current_state_ > 0) {
                        --current_state_;
                        current_color = state_[current_state_].color;
                        break_word = state_[current_state_].break_word;
                        i += 2;
                    }
                    break;
                case 'n':
                    // only inc by one as jumping to break_line only sets up the
                    // line break and then immediately continues the loop
                    ++i;
                    goto break_line;
                case CONTROL_CHAR:
                    ++i;
                    break;
                case '\0':
                default:
                    break;
            }
        }
        const char c { t[i] };
        auto& cell { cells_.at(to_index(position)) };

        if (break_word) {
            if (c == ' ' && needs_break(i + 1)) {
                is_last_char_of_line = true;
            } else {
                cell.glyph = c;
            }
        } else {
            cell.glyph = c;
        }

        if (c != ' ') {
            cell.color = current_color;
        }

        if (is_last_char_of_line) {
break_line:
            if (position.y == limit.y) {
                break;
            }
            position.x = origin.x;
            ++position.y;

            if (i + 1 < text_length && t[i + 1] == ' ') {
                // skip immediate space at new line
                ++i;
                continue;
            }
        } else {
            ++position.x;
        }
    }
}

void j_display::rectangle(const j_rect_options& options) {
    if (!in_bounds(j_vec2<uint32_t>{ options.span.right, options.span.bottom })) {
        return;
    }
    for (uint32_t y { options.span.top }; y <= options.span.bottom; y++) {
        const bool t_span { y == options.span.top };
        const bool b_span { y == options.span.bottom };

        for (uint32_t x { options.span.left }; x <= options.span.right; x++) {
            const bool l_span { x == options.span.left };
            const bool r_span { x == options.span.right };
            const auto index { to_index(j_vec2<uint32_t>{ x, y }) };

            uint32_t glyph;
            j_color color = options.color;

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

void j_display::line(j_vec2<uint32_t> from, j_vec2<uint32_t> to, uint32_t glyph, j_color color) {
    clamp(from);
    clamp(to);
    bresenham(
        static_cast<j_vec2<int32_t>>(from),
        static_cast<j_vec2<int32_t>>(to),
        [&](j_vec2<int32_t> pos) {
            auto& cell { cells_.at(to_index(pos)) };

            cell.glyph = glyph;
            cell.color = color;
        }
    );
}
