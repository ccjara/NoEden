#include "layer.hxx"

layer::layer(vector2<uint32_t> dimensions)
: dimensions(dimensions) {
    data.resize(dimensions.x * dimensions.y);

    for (auto& e: data) {
        e = std::make_unique<object>();

        e->is_solid = false;
        e->is_void = true;
    }
}

void layer::store(vector2<uint32_t> pos, object&& o) {
    const auto index { to_index(pos) };

    if (!validate_index(index)) {
        LOG(ERROR)
            << "Cannot put object at (" << pos.x << ", " << pos.y << "): "
            << "out of bounds (" << dimensions.x << ", " << dimensions.y << ")";
        return;
    }
    *data[index] = o;
}

void layer::clear() {
    data.clear();
    data.resize(dimensions.x * dimensions.y);
}

const object& layer::get(vector2<uint32_t> pos) {
    auto index = to_index(pos);

    if (!validate_index(index)) {
        return null_object;
    }
    if (!data[index]) {
        return null_object;
    }
    return *data[index];
}

inline bool layer::validate_index(size_t index) const noexcept {
    return data.size() > index;
}

inline size_t layer::to_index(const vector2<uint32_t> pos) noexcept {
    return pos.y * dimensions.x + pos.x;
}

const vector2<uint32_t>& layer::get_dimensions() noexcept {
    return dimensions;
}
