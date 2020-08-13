#include "surface.hxx"

j_surface::j_surface(std::string_view path) {
    load(path);
}

j_surface::j_surface(j_surface&& other) {
    *this = std::move(other);
}

j_surface& j_surface::operator=(j_surface&& other) {
    surf_ = std::exchange(other.surf_, nullptr);
    size_ = std::exchange(other.size_, { 0, 0 });
    return *this;
}

j_surface::~j_surface() {
    if (surf_) {
        SDL_FreeSurface(surf_);
    }
}

void j_surface::load(std::string_view path) {
    surf_ = SDL_LoadBMP(path.data());
    if (!surf_) {
        LOG(ERROR) << "Could not load surface (" << SDL_GetError() << ")";
        throw;
    }

    assert(surf_->w > 0);
    assert(surf_->h > 0);

    size_ = { static_cast<uint32_t>(surf_->w), static_cast<uint32_t>(surf_->h) };
}

void* j_surface::data() const noexcept {
    if (!surf_) {
        return nullptr;
    }
    return surf_->pixels;
}

uint32_t j_surface::width() const noexcept {
    return size_.x;
}

uint32_t j_surface::height() const noexcept {
    return size_.y;
}

j_vec2<uint32_t> j_surface::size() const noexcept {
    return size_;
}
