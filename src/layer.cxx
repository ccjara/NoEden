#include "layer.hxx"

layer::layer(size<uint32_t> s) :
    m_size(s),
    objects(s.area(), null_object) {
}

void layer::store(position<uint32_t> pos, object&& o) {
    const auto index { to_index(pos) };

    if (!validate_index(index)) {
        LOG(ERROR)
            << "Cannot put object at " << index
            << " (" << pos.x << ", " << pos.y << ")"
            << " out of bounds (" << m_size.width << ", " << m_size.height << ")";
        return;
    }
    objects[index] = std::move(o);
}

void layer::clear() {
    std::fill(objects.begin(), objects.end(), null_object);
}

const object& layer::get(position<uint32_t> pos) {
    auto index = to_index(pos);

    if (!validate_index(index)) {
        return null_object;
    }
    return objects[index];
}

inline bool layer::validate_index(size_t index) const noexcept {
    return objects.size() > index;
}

inline size_t layer::to_index(const position<uint32_t> pos) noexcept {
    return pos.y * m_size.width + pos.x;
}

const size<uint32_t>& layer::get_size() noexcept {
    return m_size;
}
