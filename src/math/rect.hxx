#ifndef JARALYN_RECT_HXX
#define JARALYN_RECT_HXX

template<typename t>
struct Rect {
    t top { 0 };
    t right { 0 };
    t bottom { 0 };
    t left { 0 };

    Rect() = default;

    Rect(t top, t right, t bottom, t left) :
        top(top),
        right(right),
        bottom(bottom),
        left(left) {
    }

    Rect(Vec2<t> pos, Vec2<t> size) :
        top(pos.y),
        right(pos.x + size.x),
        bottom(pos.y + size.y),
        left(pos.x) {
    }

    bool has_corner_at(const Vec2<t> p) const {
        return (p.x == left && p.y == top)
            || (p.x == right && p.y == top)
            || (p.x == left && p.y == bottom)
            || (p.x == right && p.y == bottom);
    }

    void expand(t length) {
        top -= length;
        right += length;
        bottom += length;
        left -= length;
    }

    void scan(const std::function<void(Vec2<t> p)> callable) const {
        for (t y { top }; y <= bottom; y++) {
            for (t x { left }; x <= right; x++) {
                callable({ x, y });
            }
        }
    }

    void limit(const Rect& other) {
        if (left < other.left) {
            left = other.left;
        }
        if (right < other.right) {
            right = other.right;
        }
        if (top < other.top) {
            top = other.top;
        }
        if (bottom < other.bottom) {
            bottom = other.bottom;
        }
    }

    [[nodiscard]] bool intersects_with(const Rect& other) const {
        return !(other.left > right
            || other.right < left
            || other.top > bottom
            || other.bottom < top
            );
    }

    [[nodiscard]] bool edges(const Vec2<t>& pos) const {
        return pos.x == left || pos.x == right || pos.y == top || pos.y == bottom;
    }

    [[nodiscard]] Vec2<t> center() const {
        return Vec2<t> {
            static_cast<t> (left + (right - left) / 2),
                static_cast<t> (top + (bottom - top) / 2)
        };
    }
};

template<typename t>
struct RectCorners {
    t top_left { 0 };
    t top_right { 0 };
    t bottom_right { 0 };
    t bottom_left { 0 };

    RectCorners() = default;

    RectCorners(t tl, t tr, t br, t bl) :
        top_left(tl),
        top_right(tr),
        bottom_right(br),
        bottom_left(bl) {
    }
};

#endif
