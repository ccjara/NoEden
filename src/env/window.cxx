#include "window.hxx"

j_window::j_window(j_vec2<uint32_t> s) : size_ { s } {
    handle_ = SDL_CreateWindow(
        "Jaralyn",
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

j_window::~j_window() {
    close();
}

j_window::j_window(j_window&& w) {
    handle_ = std::exchange(w.handle_, nullptr);
    size_ = w.size_;
}

j_window& j_window::operator=(j_window&& w) {
    handle_ = std::exchange(w.handle_, nullptr);
    size_ = w.size_;
    return *this;
}

SDL_Window* j_window::handle() const {
    return handle_;
}

j_vec2<uint32_t> j_window::size() const {
    return size_;
}

j_window::operator SDL_Window* () const {
    return handle_;
}

void j_window::resize(j_vec2<uint32_t> s) {
    size_ = s;
}

void j_window::close() {
    if (handle_ ) {
        SDL_DestroyWindow(handle_);
        handle_ = nullptr;
    }
}
