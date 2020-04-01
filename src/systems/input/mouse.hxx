#ifndef JARALYN_MOUSE_HXX
#define JARALYN_MOUSE_HXX

class mouse_t {
private:
    position<int32_t> position_;
public:
    position<int32_t> pos() const noexcept;

    int32_t x() const noexcept;
    int32_t y() const noexcept;

    /**
     * @brief Sets the mouse position (absolute)
     */
    void move(position<int32_t> pos) noexcept;
};

#endif
