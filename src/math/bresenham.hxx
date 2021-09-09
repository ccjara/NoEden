#ifndef JARALYN_BRESENHAM_HXX
#define JARALYN_BRESENHAM_HXX

// https://codereview.stackexchange.com/questions/93407/bresenham-line-drawing-implementation

/**
 * @brief Walks a line between two points and calls back on each step
 *
 * Todo: Not sure if the compiler is smart enough for the algorithm to be inlined
 * completely, including the callback. I don't think we will render many lines
 * in this game though.
 */
template<typename t, typename callable>
inline void bresenham(Vec2<t> p0, Vec2<t> p1, callable&& cb) {
    const bool swap_xy { std::abs(p1.y - p0.y) > std::abs(p1.x - p0.x) };
    if (swap_xy) {
        std::swap(p0.x, p0.y);
        std::swap(p1.x, p1.y);
    }

    const auto plot = [swap_xy, &cb](Vec2<t> p) {
        if (swap_xy) {
            cb(Vec2<t> { p.y, p.x });
        } else {
            cb(p);
        }
    };

    const Vec2<t> delta { p1 - p0 };
    const auto dx_abs { std::abs(delta.x) };
    const auto step_y { delta.y < 0 ? -1 : 1 };
    const auto step_x { delta.x < 0 ? -1 : 1 };
    auto current { p0 };

    for (auto current { p0 }; current.x != p1.x; current.x += step_x) {
        bool skip { false };
        auto error { std::abs(2 * delta.y * (current.x - p0.x) - 2 * delta.x * (current.y - p0.y)) };
        while (error > dx_abs) {
            current.y += step_y;
            error -= 2 * dx_abs;
            plot(current);
            skip = true;
        }
        if (!skip) {
            plot(current);
        }
    }
    plot(p1);
}

#endif
