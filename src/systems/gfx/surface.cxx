#include "surface.hxx"

surface::surface(const std::string& path) {
    load(path);
}

surface::~surface() {
    if (surf_) {
        SDL_FreeSurface(surf_);
    }
}

void surface::load(const std::string& path) {
    surf_ = SDL_LoadBMP("font.bmp");
    if (!surf_) {
        LOG(ERROR) << "Could not surface (" << SDL_GetError() << ")";
        throw;
    }

    assert(surf_->w > 0);
    assert(surf_->h > 0);

    size_ = { static_cast<uint32_t>(surf_->w), static_cast<uint32_t>(surf_->h) };
}

void* surface::data() const noexcept {
    assert(surf_);
    return surf_->pixels;
}

uint32_t surface::width() const noexcept {
    assert(surf_);
    return size_.width;
}

uint32_t surface::height() const noexcept {
    assert(surf_);
    return size_.height;
}

size<uint32_t> surface::get_size() const noexcept {
    return size_;
}
