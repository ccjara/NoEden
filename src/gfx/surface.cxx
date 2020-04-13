#include "surface.hxx"

j_surface::j_surface(const std::string& path) {
    load(path);
}

j_surface::~j_surface() {
    if (surf_) {
        SDL_FreeSurface(surf_);
    }
}

void j_surface::load(const std::string& path) {
    surf_ = SDL_LoadBMP("font.bmp");
    if (!surf_) {
        LOG(ERROR) << "Could not surface (" << SDL_GetError() << ")";
        throw;
    }

    assert(surf_->w > 0);
    assert(surf_->h > 0);

    size_ = { static_cast<uint32_t>(surf_->w), static_cast<uint32_t>(surf_->h) };
}

void* j_surface::data() const noexcept {
    assert(surf_);
    return surf_->pixels;
}

uint32_t j_surface::width() const noexcept {
    assert(surf_);
    return size_.width;
}

uint32_t j_surface::height() const noexcept {
    assert(surf_);
    return size_.height;
}

j_size<uint32_t> j_surface::size() const noexcept {
    return size_;
}
