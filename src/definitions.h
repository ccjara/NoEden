#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

template<class T>
struct Vector2
{
    T x;
    T y;

    Vector2(T x, T y) : x{x}, y{y} {}
};

template<class T>
struct Rect
{
    T x1;
    T y1;
    T x2;
    T y2;
};

using Vector2i = Vector2<uint32_t>;
using EventCallback = std::function<void(const SDL_Event& e)>;

using Id = size_t;

static const Id INVALID_ID = std::numeric_limits<Id>::max();

#endif
