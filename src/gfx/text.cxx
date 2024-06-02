#include "text.hxx"

Text::Text(std::string_view s, Vec2<u32> clamp) : raw_(s), clamp_(clamp) {
    update();
}

void Text::set_text(std::string_view s) {
    raw_ = s;
}

void Text::set_clamp(Vec2<u32> clamp) {
    clamp_ = clamp;
}

const std::vector<Letter>& Text::letters() const {
    return letters_;
}

void Text::update() {
    letters_.clear();
    if (!clamp_.x || !clamp_.y) {
        return;
    }
    Vec2<u32> offset;
    const size_t text_length { raw_.length() };
    static constexpr char CONTROL_CHAR = '$';

    static constexpr size_t MAX_STATES { 8 };
    std::array<TextState, MAX_STATES> states;
    TextState* state { states.data() };
    TextState* const first_state { &states.front() };
    TextState* const last_state { &states.back() };

    auto push_state_copy = [&]() -> bool {
        if (state == last_state) {
            return false;
        }
        TextState* const next_state = state + 1;
        *next_state = *state;
        state = next_state;
        return true;
    };

    const auto parse_color = [](std::string_view text) -> Color {
        i32 hex_color { 0xFFFFFF };
        // note: from_chars processes the interval [first,last),
        //       so the null terminator must be included
        std::from_chars(&text.front(), &text.back() + 1, hex_color, 16);
        return Color(hex_color);
    };

    // REFACTOR!

    const auto needs_break = [&](size_t i) -> bool {
        auto x { offset.x };

        while (x <= clamp_.x) {
            if (i >= text_length) {
                return false;
            }
            switch (raw_[i]) {
            case CONTROL_CHAR:
                switch (raw_[i + 1]) {
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
        if (raw_[i] == CONTROL_CHAR) {
            switch (raw_[i + 1]) {
                case 'c': {
                    //      n: 1234567
                    // t[i+n]: cRRGGBB
                    if (i + 7 >= text_length || !push_state_copy()) {
                        break;
                    }
                    state->color = parse_color(std::string_view(&raw_[i + 2], 6));
                    i += 7;
                    continue;
                }
                case 'w':
                case 'a':
                    if (push_state_copy()) {
                        state->break_word = raw_[i + 1] == 'w';
                        ++i;
                        continue;
                    }
                    break;
                case '!':
                    if (state != first_state) {
                        --state;
                        ++i;
                        continue;
                    }
                    break;
                case 'n':
                    ++i;
                    offset.x = 0;
                    ++offset.y;
                    continue;
                case CONTROL_CHAR:
                    ++i;
                    break;
                case '\0':
                default:
                    break;
            }
        }
        auto& letter { letters_.emplace_back() };
        const char c { raw_[i] };
        bool is_last_char_of_line { offset.x == clamp_.x };

        if (!is_last_char_of_line && state->break_word) {
            if (c == ' ' && needs_break(i + 1)) {
                is_last_char_of_line = true;
            } else {
                letter.glyph = c;
            }
        } else {
            letter.glyph = c;
        }

        if (c != ' ') {
            letter.color = state->color;
        }

        if (is_last_char_of_line) {
            offset.x = 0;
            ++offset.y;
            letter.offset = offset;

            if (i + 1 < text_length && raw_[i + 1] == ' ') {
                // skip immediate space at new line
                ++i;
                continue;
            }
        } else {
            ++offset.x;
            letter.offset = offset;
        }
    }
}
