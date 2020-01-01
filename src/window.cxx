#include "window.hxx"

window::window(window_size size) :
    size(size) {
    handle = SDL_CreateWindow(
        "Jaralyn",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        static_cast<int> (size.x),
        static_cast<int> (size.y),
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (handle == nullptr) {
        LOG(ERROR) << "Window could not be created. " << SDL_GetError();
        throw;
    }
}

window::~window() noexcept
{
    if (handle != nullptr) {
        SDL_DestroyWindow(handle);
        handle = nullptr;
    }
}

SDL_Window* window::get_handle() const noexcept
{
    return handle;
}

window_size window::get_size() const noexcept
{
    return size;
}

void window::resize(window_size s) noexcept
{
    size = s;
}
