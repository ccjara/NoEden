#ifndef NOEDEN_UI_WINDOW_HXX
#define NOEDEN_UI_WINDOW_HXX

#include "ui_node.hxx"

class UiWindow : public UiNode {
public:
    /**
     * @brief Readonly accessor of the title_ attribute
     */
    std::string_view title() const;

    /**
     * @brief Writing accessor of the title_ attribute
     */
    void set_title(std::string_view title);

    UiWindow();
private:
    /**
     * @brief An optional string that will be displayed as a title
     */
    std::string title_;
};

#endif
