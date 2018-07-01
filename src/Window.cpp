#include "stdafx.h"

Window::Window(const WindowSize& windowSize) :
    handle(nullptr),
    size(windowSize)
{
    handle = SDL_CreateWindow(
        "Trinias",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        static_cast<int> (size.x),
        static_cast<int> (size.y),
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (handle == nullptr) {
        LOG(ERROR) << "Window could not be created." << SDL_GetError();
        throw;
    }
}

Window::~Window()
{
    if (handle != nullptr) {
        SDL_DestroyWindow(handle);
    }
}

SDL_Window* Window::getHandle() const noexcept
{
    return handle;
}

const WindowSize Window::getSize() const noexcept
{
    return size;
}

void Window::adjust(const WindowSize& s) noexcept
{
    size = s;
}
