#include "stdafx.h"

Layer::Layer(Vector2<uint32_t> dimensions)
: dimensions(dimensions) {
    data.resize(dimensions.x * dimensions.y);
}

void Layer::put(Vector2<uint32_t> pos, Object* o) {
    auto index = toIndex(pos);

    if (!validateIndex(index)) {
        LOG(ERROR)
            << "Cannot put object at (" << pos.x << ", " << pos.y << "): "
            << "out of bounds (" << dimensions.x << ", " << dimensions.y << ")";
        return;
    }

    data[index] = std::unique_ptr<Object>(o);
}

void Layer::clear() {
    data.clear();
    data.resize(dimensions.x * dimensions.y);
}

Object* const Layer::get(Vector2<uint32_t> pos) {
    auto index = toIndex(pos);

    if (!validateIndex(index)) {
        return &nullObject;
    }
    auto element = data[index].get();

    if (element == nullptr) {
        return &nullObject;
    }
    return element;
}

inline bool Layer::validateIndex(const size_t index) noexcept {
    return data.size() > index;
}

inline const size_t Layer::toIndex(const Vector2<uint32_t> pos) noexcept {
    return pos.y * dimensions.x + pos.x;
}

const Vector2<uint32_t> Layer::getDimensions() noexcept {
    return dimensions;
}
