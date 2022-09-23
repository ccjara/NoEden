#include "window.hxx"

void Window::open(Vec2<u32> size, const char *title) {
    if (handle_) {
        Log::error("Window is already open");
        return;
    }
    size_ = size;
    title_ = title;

    handle_ = SDL_CreateWindow(
        title_.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        static_cast<int> (size_.x),
        static_cast<int> (size_.y),
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (handle_ == nullptr) {
        Log::error("Could not create window: {}", SDL_GetError());
        std::abort();
    }
}

void Window::close() {
    if (handle_ ) {
        SDL_DestroyWindow(handle_);
        handle_ = nullptr;
    }
}

SDL_Window* Window::handle() {
    return handle_;
}

Vec2<u32> Window::size() {
    return size_;
}

void Window::resize(Vec2<u32> size) {
    size_ = size;
}

