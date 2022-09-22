#ifndef JARALYN_TEXT_HXX
#define JARALYN_TEXT_HXX

#include "display_cell.hxx"

struct Letter {
    /**
     * @brief Glyph to render
     */
    i32 glyph { 0 };

    /**
     * @brief Color of this letter
     */
    Color color = Color::white();

    /**
     * @brief Offset that will be added to the actual position of the text to render
     */
    Vec2<i32> offset;
};

class Text {
public:
    explicit Text() = default;
    explicit Text(std::string_view s, Vec2<u32> clamp);

    void set_text(std::string_view s);
    void set_clamp(Vec2<u32> clamp);
    void update();

    [[nodiscard]] const std::vector<Letter>& letters() const;
private:
    struct TextState {
        Color color { Color::white() };
        bool break_word { true };
    };

    std::vector<Letter> letters_;
    std::string raw_;
    Vec2<u32> clamp_;
};

#endif
