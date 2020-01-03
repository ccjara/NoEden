#ifndef JARALYN_ROOM_GEN_HXX
#define JARALYN_ROOM_GEN_HXX

#include "../ide_pch.hxx"

#include "../layer.hxx"

struct room_gen_config {
    uint32_t max_room_attempts { 50 };
    uint32_t min_room_size_xy { 3 };
    uint32_t max_room_size_xy { 5 };
};

class room_gen
{
private:
    rng_t rng;
public:
    room_gen() : rng(get_rng()) {
    }

    void generate(layer& layer, const room_gen_config& config);

    void generate_test_arena(layer& layer);
};

#endif