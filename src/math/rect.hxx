#ifndef NOEDEN_RECT_HXX
#define NOEDEN_RECT_HXX

template<typename t>
struct Rect {
    t x1 { 0 };
    t y1 { 0 };
    t x2 { 0 };
    t y2 { 0 };

    Rect() = default;

    Rect(t x1, t y1, t x2, t y2) :
        x1(x1),
        y1(y1),
        x2(x2),
        y2(y2) {
    }

    Rect(Vec2<t> pos, Vec2<t> size) :
        x1(pos.x),
        x2(pos.x + size.x),
        y1(pos.y),
        y2(pos.y + size.y) {
    }

    bool has_corner_at(const Vec2<t> p) const {
        return (p.x == x1 && p.y == y1)
            || (p.x == x2 && p.y == y1)
            || (p.x == x1 && p.y == y2)
            || (p.x == x2 && p.y == y2);
    }

    void expand(t length) {
        y1 -= length;
        x2 += length;
        y2 += length;
        x1 -= length;
    }

    void scan(const std::function<void(Vec2<t> p)> callable) const {
        for (t y { y1 }; y <= y2; y++) {
            for (t x { x1 }; x <= x2; x++) {
                callable({ x, y });
            }
        }
    }

    void limit(const Rect& other) {
        if (x1 < other.x1) {
            x1 = other.x1;
        }
        if (x2 < other.x2) {
            x2 = other.x2;
        }
        if (y1 < other.y1) {
            y1 = other.y1;
        }
        if (y2 < other.y2) {
            y2 = other.y2;
        }
    }

    [[nodiscard]] bool intersects_with(const Rect& other) const {
        return !(other.x1 > x2
            || other.x2 < x1
            || other.y1 > y2
            || other.y2 < y1
            );
    }

    [[nodiscard]] bool edges(const Vec2<t>& pos) const {
        return pos.x == x1 || pos.x == x2 || pos.y == y1 || pos.y == y2;
    }

    [[nodiscard]] Vec2<t> center() const {
        return Vec2<t> {
            static_cast<t> (x1 + (x2 - x1) / 2),
                static_cast<t> (y1 + (y2 - y1) / 2)
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
