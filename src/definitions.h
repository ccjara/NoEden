#ifndef __DEFINITIONS_H__
#define __DEFINITIONS_H__

template<class T>
struct Vector2
{
    T x;
    T y;
};

using EventCallback = std::function<void(const SDL_Event& e)>;

#endif
