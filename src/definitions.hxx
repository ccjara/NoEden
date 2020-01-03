#ifndef JARALYN_DEFINITIONS_HXX
#define JARALYN_DEFINITIONS_HXX

template<class T>
struct vector2
{
    T x;
    T y;
};

template<typename t>
struct size {
    t width { 0 };
    t height { 0 };

    [[nodiscard]] t area() const noexcept { return width * height; }
};

template<typename t>
struct position {
    t x { 0 };
    t y { 0 };
};

template<typename t>
struct rect {
    t top { 0 };
    t right { 0 };
    t bottom { 0 };
    t left { 0 };

    rect() = default;

    rect(position<t> pos, size<t> size) :
        top(pos.y),
        right(pos.x + size.width),
        bottom(pos.y + size.height),
        left(pos.x) {
    }

    bool has_corner_at(const position<t> p) const noexcept {
        return (p.x == left && p.y == top)
            || (p.x == right && p.y == top)
            || (p.x == left && p.y == bottom)
            || (p.x == right && p.y == bottom);
    }

    void expand(t length) noexcept {
        top -= 1;
        right += 1;
        bottom += 1;
        left -= 1;
    }

    void scan(const std::function<void(position<t> p)> callable) const {
        for (t y { top }; y <= bottom; y++) {
            for (t x { left }; x <= right; x++) {
                callable({ x, y });
            }
        }
    }

    [[nodiscard]] bool intersects_with(const rect& other) const noexcept {
        return !(other.left > right
                 || other.right < left
                 || other.top > bottom
                 || other.bottom < top
        );
    }

    [[nodiscard]] bool edges(const position<t>& pos) const noexcept {
        return pos.x == left || pos.x == right || pos.y == top || pos.y == bottom;
    }

    [[nodiscard]] position<t> center() const noexcept {
        return position<t> {
            static_cast<t> (left + (right - left) / 2),
            static_cast<t> (top + (bottom - top) / 2)
        };
    }
};

#endif
