#ifndef __LAYER_H__
#define __LAYER_H__

class Layer {
private:
    Vector2<uint32_t> dimensions;
    Object nullObject;

    inline bool validateIndex(size_t index) noexcept;
    inline const size_t toIndex(const Vector2<uint32_t> pos) noexcept;
public:
    std::vector<std::unique_ptr<Object>> data;

    Layer(Vector2<uint32_t> dimensions);

    // FIXME: don't know how to enforce strict unique ownership on o
    void put(Vector2<uint32_t> pos, Object* o);
    Object* const get(Vector2<uint32_t> pos);
    void clear();

    const Vector2<uint32_t> getDimensions() noexcept;
};

#endif