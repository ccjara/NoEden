#include "display.hxx"

void Display::text(const Text& text) {
    const auto& ops = text.ops();

    for (size_t op_index = 0; op_index < ops.size(); ++op_index) {
        const auto& op = ops[op_index];
        const auto glyph_count = op.glyphs.size();
        for (size_t i = 0; i < glyph_count; ++i) {
            const i32 glyph = op.glyphs[i];
            const Vec2<u32> pos = op.position + Vec2<i32>(i, 0);
            put(DisplayCell{ glyph, op.color }, pos);
        }
    }
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
                        ++i;
                        continue;
                    }
                    break;
                case '!':
                    if (state_ != first_state_) {
                        --state_;
                        ++i;
                        continue;
                    }
                    break;
                case 'n':
                    ++i;
                    if (position.y == limit.y) {
                        return;
                    }
                    position.x = origin.x;
                    ++position.y;
                    continue;
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
    Vec2<i32> pos;

    for (pos.y = options.span.y1; pos.y <= options.span.y2; ++pos.y) {
        const bool y_min { pos.y == options.span.y1 };
        const bool y_max { pos.y == options.span.y2 };

        for (pos.x = options.span.x1; pos.x <= options.span.x2; ++pos.x) {
            const bool x_min { pos.x == options.span.x1 };
            const bool x_max { pos.x == options.span.x2 };
            const size_t index = to_index(pos);
            if (!in_bounds(index)) {
                continue;
            }
            u32 glyph;
            Color color = options.color;

            if (x_min) {
                if (y_min) {
                    glyph = options.corner_glyphs.top_left;
                } else if (y_max) {
                    glyph = options.corner_glyphs.bottom_left;
                } else {
                    glyph = options.border_glyphs.x1;
                }
            } else if (x_max) {
                if (y_min) {
                    glyph = options.corner_glyphs.top_right;
                } else if (y_max) {
                    glyph = options.corner_glyphs.bottom_right;
                } else {
                    glyph = options.border_glyphs.x2;
                }
            } else if (y_min) {
                glyph = options.border_glyphs.y1;
            } else if (y_max) {
                glyph = options.border_glyphs.y2;
            } else {
                if (!options.fill_color) {
                    continue;
                }
                glyph = 745U;
                color = options.fill_color.value();
            }
            auto& cell = cells_.at(index);
            cell.color = color;
            cell.glyph = glyph;
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
