#include "window.hxx"

Window::~Window() {
    close();
}

void Window::open(Vec2<u32> size, const char *title) {
    if (handle_) {
        LOG(ERROR) << "Window is already open";
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
        LOG(ERROR) << "Could not create window: " << SDL_GetError();
        std::abort();
    }
}

void Window::close() {
    if (handle_ ) {
        SDL_DestroyWindow(handle_);
        handle_ = nullptr;
    }
}

SDL_Window* Window::handle() const {
    return handle_;
}

Vec2<u32> Window::size() const {
    return size_;
}

Window::operator SDL_Window* () const {
    return handle_;
}

void Window::resize(Vec2<u32> size) {
    size_ = size;
}

