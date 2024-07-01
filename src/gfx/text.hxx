#ifndef NOEDEN_TEXT_HXX
#define NOEDEN_TEXT_HXX

struct TextOp {
    /**
     * @brief Slice of text to render in this block as glyphs
     */
    std::vector<i32> glyphs;

    /**
     * @brief Color to use when rendering the text
     */
    Color color = Color::white();

    /**
     * @brief Position (offset) at which to render
     */
    Vec2<i32> position = {0, 0};

    /**
     * @brief Whether this text op will end with a new line
     */
    bool new_line = false;
};

class Text {
public:
    void set_text(std::string_view text);
    void set_region(Vec2<i32> region);
    void update();

    const std::vector<TextOp>& ops() const;
private:
    bool dirty_ = false;

    std::vector<TextOp> ops_ = {};
    std::string text_ = "";
    Color color_ = Color::white();
    Vec2<i32> region_ = {0, 0};
};

#endif
