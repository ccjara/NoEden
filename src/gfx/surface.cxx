#include "surface.hxx"

Surface::Surface(std::string_view path) {
    load(path);
}

Surface::Surface(Surface&& other) {
    *this = std::move(other);
}

Surface& Surface::operator=(Surface&& other) {
    surf_ = std::exchange(other.surf_, nullptr);
    size_ = std::exchange(other.size_, { 0, 0 });
    return *this;
}

Surface::~Surface() {
    if (surf_) {
        SDL_FreeSurface(surf_);
    }
}

void Surface::load(std::string_view path) {
    surf_ = SDL_LoadBMP(path.data());
    if (!surf_) {
        LOG(ERROR) << "Could not load surface (" << SDL_GetError() << ")";
        std::abort();
    }

    assert(surf_->w > 0);
    assert(surf_->h > 0);

    size_ = { static_cast<u32>(surf_->w), static_cast<u32>(surf_->h) };
}

void* Surface::data() const {
    if (!surf_) {
        return nullptr;
    }
    return surf_->pixels;
}

u32 Surface::width() const {
    return size_.x;
}

u32 Surface::height() const {
    return size_.y;
}

Vec2<u32> Surface::size() const {
    return size_;
}
