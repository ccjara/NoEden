#include "text.hxx"

static constexpr char COMMAND_CHAR = '$';
static constexpr char COMMAND_GLYPH = 'g';
static constexpr char COMMAND_COLOR = 'c';
static constexpr char COMMAND_NEWLINE = 'n';
static constexpr char COMMAND_RESTORE_COLOR = 'r';

void Text::set_text(std::string_view text) {
    dirty_ = text != text_;
    text_ = text;
}

void Text::set_region(glm::ivec2 region) {
    dirty_ = region_ != region;
    region_ = region;
}

void Text::set_color(Color color) {
    dirty_ = color_ != color;
    color_ = color;
}

void Text::update() {
    if (!dirty_) {
        return;
    }
    ops_.clear();
    if (text_.empty() || !region_.x || !region_.y) {
        return;
    }

    if (text_.size() == 1 && text_[0] == COMMAND_CHAR) {
        return;
    }

    TextOp* current_op = &ops_.emplace_back();
    current_op->color = color_;

    const i32 max_index = static_cast<i32>(text_.length() - 1);
    std::stack<Color> color_stack;

    for (i32 i = 0; i <= max_index; ++i) {
        if (text_[i] != COMMAND_CHAR) {
            current_op->glyphs.push_back(text_[i]);
            continue;
        }

        if (i >= max_index) {
            // "$" at the end of the text - invalid - to output a $ sign, $$ must be used
            break;
        }

        // i is at the command char ($)
        // continuing from a branch is used for inline commands and will not create a new op
        // breaking from a branch will create a new op
        switch (text_[i + 1]) {
           case COMMAND_COLOR: {
                // $cRRGGBB
                const auto color_start = i + 2;
                const auto color_end = i + 7;

                if (color_end >= max_index) {
                    // remaining text is equal (noop) or shorter (corrupted) than the control sequence. do not print it
                    goto EXIT;
                }
                color_stack.push(Color::from_string(std::string_view(&text_[color_start], 6)));
                i = color_end;
                break;
            }
            case COMMAND_RESTORE_COLOR:
                // $r
                if (!color_stack.empty()) {
                    color_stack.pop();
                }
                i += 1;
                break;
            case COMMAND_NEWLINE:
                // $n
                current_op->new_line = true;
                i += 1;
                break;
            case COMMAND_GLYPH: {
                // $g0000
                if (i + 4 >= max_index) {
                    // remaining text shorter (corrupted) than the control sequence. do not print it
                    goto EXIT;
                }

                i32 glyph = 0;
                const char* first = &text_[i + 2];
                const char* last = &text_[i + 6];
                const auto result = std::from_chars(first, last, glyph, 16);

                if (result.ec == std::errc() && result.ptr == last) {
                    current_op->glyphs.push_back(glyph);
                }

                i += 5;
                continue;
            }
            default:
                // escaped `$$` or unknown command char: skip only the command char
                current_op->glyphs.push_back(text_[i + 1]);
                ++i;
                continue;
        }

        if (!current_op->glyphs.empty()) {
            // no need to create a new op if the current one is empty
            // this happens if a op-creating command is used at the beginning of the text or back to back
            current_op = &ops_.emplace_back();
        }

        // problematic code placement: should go into the color command block later
        // when the color command is placed in a way that it does not create
        // a new op then the color command would be skipped if this code was not here
        if (color_stack.empty()) {
            current_op->color = color_;
        } else {
            current_op->color = color_stack.top();
        }
    }
EXIT:

    // positioning
    i32 last_length = 0;
    for (auto& op : ops_) {
        op.position.x = last_length;
        last_length += static_cast<i32> (op.glyphs.size());
    }
}

const std::vector<TextOp>& Text::ops() const {
    return ops_;
}
