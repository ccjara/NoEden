#ifndef NOEDEN_UI_TEXT_HXX
#define NOEDEN_UI_TEXT_HXX

#include "ui_node.hxx"
#include "../gfx/text.hxx"

class UiText : public UiNode {
public:
    /**
     * @brief Provides access to the underlying Text instance
     */
    Text& text() const;

    void resize(Vec2<u32> size) final override;
    void resize(u32 width, u32 height) final override;
    void set_width(u32 width) final override;
    void set_height(u32 height) final override;

    /**
     * @brief Forces an update of the text buffer
     */
    void update();

    UiText();
private:
    /**
     * @brief The content this node should display when rendered
     */
    std::unique_ptr<Text> text_;
};

#endif
