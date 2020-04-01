#include "window.hxx"

window::window(size<uint32_t> s) : size_ { s } {
    handle_ = SDL_CreateWindow(
        "Jaralyn",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        static_cast<int> (size_.width),
        static_cast<int> (size_.height),
        SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (handle_ == nullptr) {
        LOG(ERROR) << "Could not create window: " << SDL_GetError();
        throw;
    }
}

window::~window() noexcept {
    if (handle_ != nullptr) {
        SDL_DestroyWindow(handle_);
    }
}

window::window(window&& w) noexcept {
    handle_ = std::exchange(w.handle_, nullptr);
    size_ = w.size_;
}

window& window::operator=(window&& w) noexcept {
    handle_ = std::exchange(w.handle_, nullptr);
    size_ = w.size_;
    return *this;
}

SDL_Window* window::get_handle() const noexcept {
    return handle_;
}

size<uint32_t> window::get_size() const noexcept {
    return size_;
}

void window::resize(size<uint32_t> s) noexcept {
    size_ = s;
}
