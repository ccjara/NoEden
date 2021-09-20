#include "display.hxx"

Display::Display() : Grid<DisplayCell>(DisplayCell{}) {
}

void Display::text(std::string_view t, Vec2<u32> position, Vec2<u32> clamp) {
    state_ = first_state_; // reset state stack

    const Vec2<u32> limit {
        std::min(clamp.x, dimensions_.x),
        std::min(clamp.y, dimensions_.y)
    };
    if (!in_bounds(position) || limit.x < position.x || limit.y < position.y) {
        return;
    }
    const auto origin { position };
    const size_t text_length { t.length() };

    // REFACTOR!

    const auto needs_break = [&](size_t i) -> bool {
        auto x { position.x };

        while (x <= limit.x) {
            if (i >= text_length) {
                return false;
            }
            switch (t[i]) {
            case CONTROL_CHAR:
                switch (t[i + 1]) {
                    // can increase `i` safely past boundaries as it is checked on each iteration
                    case 'c':
                        i += 8;
                        continue;
                    case '!':
                    case 'w':
                    case 'a':
                        i += 2;
                        continue;
                    case 'n':
                        i += 2;
                        return false;
                    case '\0':
                    default:
                        return false;
                }
                continue;
            case ' ':
                return false;
            }
            ++x;
            ++i;
        }
        return true;
    };

    bool immediate_break = false;

    for (size_t i { 0 }; i < text_length; ++i) {
        if (t[i] == CONTROL_CHAR) {
            switch (t[i + 1]) {
                case 'c': {
                    //      n: 1234567
                    // t[i+n]: cRRGGBB
                    if (i + 7 >= text_length || state_ == last_state_)
                        break;
                    push_copy();
                    state_->color = parse_color(std::string_view(&t[i + 2], 6));
                    i += 7;
                    continue;
                }
                case 'w':
                case 'a':
                    if (state_ != last_state_) {
                        push_copy();
                        state_->break_word = t[i + 1] == 'w';
                        i += 1;
                        continue;
                    }
                    break;
                case '!':
                    if (state_ != first_state_) {
                        --state_;
                        i += 1;
                        continue;
                    }
                    break;
                case 'n':
                    // only inc by one as jumping to break_line only sets up the
                    // line break and then immediately continues the loop
                    ++i;
                    immediate_break = true;
                case CONTROL_CHAR:
                    ++i;
                    break;
                case '\0':
                default:
                    break;
            }
        }

        if (immediate_break) {
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
            return;
        }

        const char c { t[i] };
        auto& cell { cells_.at(to_index(position)) };
        bool is_last_char_of_line { position.x == limit.x };

        if (!is_last_char_of_line && state_->break_word) {
            if (c == ' ' && needs_break(i + 1)) {
                is_last_char_of_line = true;
            } else {
                cell.glyph = c;
            }
        } else {
            cell.glyph = c;
        }

        if (c != ' ') {
            cell.color = state_->color;
        }

        if (is_last_char_of_line) {
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

void Display::rectangle(const RectOptions& options) {
    if (!in_bounds(Vec2<u32>{ options.span.right, options.span.bottom })) {
        return;
    }
    for (u32 y { options.span.top }; y <= options.span.bottom; y++) {
        const bool t_span { y == options.span.top };
        const bool b_span { y == options.span.bottom };

        for (u32 x { options.span.left }; x <= options.span.right; x++) {
            const bool l_span { x == options.span.left };
            const bool r_span { x == options.span.right };
            const auto index { to_index(Vec2<u32>{ x, y }) };

            u32 glyph;
            Color color = options.color;

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

void Display::line(Vec2<u32> from, Vec2<u32> to, u32 glyph, Color color) {
    clamp(from);
    clamp(to);
    bresenham(
        static_cast<Vec2<i32>>(from),
        static_cast<Vec2<i32>>(to),
        [&](Vec2<i32> pos) {
            auto& cell { cells_.at(to_index(pos)) };

            cell.glyph = glyph;
            cell.color = color;
        }
    );
}

inline Color Display::parse_color(std::string_view text) const {
    // not really sure why std::from_chars does not have a string_view friendly
    // and constexpr interface (C++23 maybe?)
    i32 hex_color { 0xFFFFFF };
    // note: from_chars processes the interval [first,last),
    //       so the null terminator must be included
    std::from_chars(&text.front(), &text.back() + 1, hex_color, 16);
    return Color(hex_color);
}

inline void Display::push_copy() {
    TextState* const next_state { state_ + 1 };
    *next_state = *state_;
    state_ = next_state;
}
