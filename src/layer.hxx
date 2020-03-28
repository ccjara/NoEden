#ifndef JARALYN_LAYER_HXX
#define JARALYN_LAYER_HXX

#include "object.hxx"

class layer {
private:
    size<uint32_t> m_size;

    [[nodiscard]] inline bool validate_index(size_t index) const noexcept;
    [[nodiscard]] inline size_t to_index(position<uint32_t> pos) noexcept;
public:
    std::vector<object> objects;

    explicit layer(size<uint32_t> s);

    void store(position<uint32_t> pos, object&& o);
    const object& get(position<uint32_t> pos);
    void clear();

    const size<uint32_t>& get_size() noexcept;
};

#endif