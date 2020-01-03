#include "room_gen.hxx"

void room_gen::generate(layer& layer, const room_gen_config& config) {
    const auto ls { layer.get_size() };
    if (!ls.area()) {
        return;
    }

    std::uniform_int_distribution<uint32_t> binary_dist { 0, 1 };
    std::uniform_int_distribution<uint32_t> room_size_dist {
        config.min_room_size_xy,
        config.max_room_size_xy
    };

    using room_t = rect<uint32_t>;

    std::vector<room_t> rooms;

    for (uint32_t attempt = 1; attempt <= config.max_room_attempts; attempt++) {
        auto square_length { room_size_dist(rng) * 2 + 1 };

        size<uint32_t> room_size { square_length, square_length };

        std::uniform_int_distribution<uint32_t> odd_dist {
            0,
            1 + static_cast<uint32_t> (square_length / 2)
        };

        if (binary_dist(rng) == 0) {
            room_size.width += odd_dist(rng) * 2;
        } else {
            room_size.height += odd_dist(rng) * 2;
        }

        const auto max_x { static_cast<uint32_t> (ls.width - room_size.width) / 2 - 2 };
        const auto max_y { static_cast<uint32_t> (ls.height - room_size.height) / 2 - 2 };
        std::uniform_int_distribution<uint32_t> x_dist { 0, max_x };
        std::uniform_int_distribution<uint32_t> y_dist { 0, max_y };

        position<uint32_t> pos { x_dist(rng) * 2 + 1, y_dist(rng) * 2 + 1 };
        room_t new_room { pos, room_size };
        room_t new_room_padded { new_room };

        new_room_padded.expand(1);

        if (std::none_of(
            rooms.begin(),
            rooms.end(),
            [&new_room_padded](const room_t& existing_room) {
                return new_room_padded.intersects_with(existing_room);
            }
        )) {
            rooms.push_back(std::move(new_room));
        }
    }

    for (const auto& room: rooms) {
        room.scan([&room, &layer](position<uint32_t> p) {
            object obj;
            obj.is_void = false;
            obj.is_solid = room.edges(p);

            layer.store(p, std::move(obj));
        });
    }

    // carve some tunnels
    std::vector<room_t> unconnected_rooms { rooms };

    std::shuffle(unconnected_rooms.begin(), unconnected_rooms.end(), rng);

    const auto unfold_perimeter = [](const room_t& room) {
        std::vector<position<uint32_t>> points;
        // Optimize: (walks the entire area, not just the perimeter)
        room.scan([&room, &points](position<uint32_t> p) {
            if (room.edges(p) && !room.has_corner_at(p)) {
                points.push_back(p);
            }
        });
        return points;
    };

    while (unconnected_rooms.size() >= 2) {
        room_t r1 { unconnected_rooms.back() };
        unconnected_rooms.pop_back();
        room_t r2 { unconnected_rooms.back() };
        unconnected_rooms.pop_back();

        const auto r1_perimeter { unfold_perimeter(r1) };
        const auto r2_perimeter { unfold_perimeter(r2) };

        if (!r1_perimeter.size() || !r2_perimeter.size()) {
            LOG(ERROR) << "NO PERIMETER???";
            break;
        }

        std::uniform_int_distribution<uint32_t> r1_perim_dist {
            0,
            static_cast<uint32_t> (r1_perimeter.size() - 1)
        };
        std::uniform_int_distribution<uint32_t> r2_perim_dist {
            0,
            static_cast<uint32_t> (r2_perimeter.size() - 1)
        };

        auto r1_door_pos { r1_perimeter[r1_perim_dist(rng)] };
        auto r2_door_pos { r2_perimeter[r2_perim_dist(rng)] };

        object obj_r1_center;
        obj_r1_center.is_void = false;
        obj_r1_center.debug_char = 'C';
        layer.store(r1.center(), std::move(obj_r1_center));
        object obj_r2_center;
        obj_r2_center.is_void = false;
        obj_r2_center.debug_char = 'C';
        layer.store(r2.center(), std::move(obj_r2_center));

        object obj_door1;
        obj_door1.is_void = false;
        obj_door1.is_solid = false;
        obj_door1.debug_char = '1';
        layer.store(r1_door_pos, std::move(obj_door1));

        object obj_door2;
        obj_door2.is_void = false;
        obj_door2.is_solid = false;
        obj_door2.debug_char = '2';
        layer.store(r2_door_pos, std::move(obj_door2));
    }
}

void room_gen::generate_test_arena(layer& layer) {
    rect<uint32_t> room { { 3, 3 }, { 53, 31 } };

    room.scan([&room, &layer](position<uint32_t> p) {
        object obj;
        obj.is_void = false;
        obj.is_solid = room.edges(p);

        layer.store(p, std::move(obj));
    });
}