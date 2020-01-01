#ifndef JARALYN_LAYER_HXX
#define JARALYN_LAYER_HXX

#include "ide_pch.hxx"

#include "object.hxx"

class layer {
private:
    vector2<uint32_t> dimensions;
    object null_object;

    [[nodiscard]] inline bool validate_index(size_t index) const noexcept;
    [[nodiscard]] inline size_t to_index(vector2<uint32_t> pos) noexcept;
public:
    std::vector<std::unique_ptr<object>> data;

    explicit layer(vector2<uint32_t> dimensions);

    void store(vector2<uint32_t> pos, object&& o);
    const object& get(vector2<uint32_t> pos);
    void clear();

    const vector2<uint32_t>& get_dimensions() noexcept;
};

#endif