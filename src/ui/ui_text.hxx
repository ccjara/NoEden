#ifndef JARALYN_UI_TEXT_HXX
#define JARALYN_UI_TEXT_HXX

#include "ui_node.hxx"

class UiText : public UiNode {
public:
    /**
     * @brief Readonly accessor of the title_ attribute
     */
    std::string_view text() const;

    /**
     * @brief Writing accessor of the title_ attribute
     */
    void set_text(std::string_view title);

    UiText();
private:
    /**
     * @brief The content this node should display when rendered
     */
    std::string text_;
};

#endif
