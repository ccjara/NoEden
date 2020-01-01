#ifndef JARALYN_ROOM_GEN_HXX
#define JARALYN_ROOM_GEN_HXX

#include "../ide_pch.hxx"

#include "../layer.hxx"

namespace {
    enum class direction {
        horizontal,
        vertical,
    };
}

struct partition {
    vector2<uint32_t> pos = vector2<uint32_t> {0, 0 };
    vector2<uint32_t> size = vector2<uint32_t> {0, 0 };
    std::unique_ptr<partition> first = nullptr;
    std::unique_ptr<partition> second = nullptr;
    uint32_t depth = 0;
};

class room_gen
{
public:
    partition test_partition;

    void generate(layer& layer, uint32_t max_depth);
};

#endif