#include "room_gen.hxx"

void room_gen::generate(layer& layer, uint32_t max_depth) {
    auto rng { get_rng() };

    layer.clear();

    std::uniform_int_distribution<uint32_t> room_count_rng(1, 11);

    const auto dim { layer.get_dimensions() };

    // partition
    test_partition.pos = vector2<uint32_t> {0, 0 };
    test_partition.size = dim;
    test_partition.first = nullptr;
    test_partition.second = nullptr;
    test_partition.depth = 0;

    const auto MAX_DEPTH { max_depth };
    const auto PADDING { 1u };
    const auto MIN_ROOM_LENGTH { 1u + 2 * PADDING };

    std::uniform_int_distribution<uint8_t> dist_coinflip(0, 1);

    std::function<void(partition&)> subdivide = [&](partition &part) {
        if (part.depth >= MAX_DEPTH) {
            return;
        }
        // random subdivision
        const auto direction {dist_coinflip(rng) == 0 ? direction::horizontal : direction::vertical };
        const auto length {direction == direction::horizontal ? part.size.y : part.size.x };

        if (length < 2 * MIN_ROOM_LENGTH * PADDING) {
            return;
        }

        const auto center { static_cast<uint32_t> (length / 2) };

        std::poisson_distribution<uint32_t> center_deviation_dist(center);

        auto divisionOffset { center_deviation_dist(rng) };

        // every division must produce two rooms of at least MIN_ROOM_LENGTH
        // this constraints extreme outliers of the distribution function
        // FIXME: this is buggy
        divisionOffset = std::max(MIN_ROOM_LENGTH, divisionOffset);
        divisionOffset = std::min(length - MIN_ROOM_LENGTH, divisionOffset);

        divisionOffset = center;

        part.first = std::make_unique<partition>();
        part.second = std::make_unique<partition>();

        part.first->depth = part.second->depth = part.depth + 1;
        part.first->pos = part.pos;

        if (direction == direction::horizontal) {
            part.first->size = { part.size.x, divisionOffset };
            part.second->pos = { part.pos.x, part.pos.y + divisionOffset };
            part.second->size = { part.size.x, part.size.y - divisionOffset };
        } else {
            part.first->size = { divisionOffset, part.size.y };
            part.second->pos = { part.pos.x + divisionOffset, part.pos.y };
            part.second->size = { part.size.x - divisionOffset, part.size.y };
        }
        subdivide(*part.first);
        subdivide(*part.second);
    };
    subdivide(test_partition);

    // 1. shrink partition rect by random amount
    // 2. determine maximum free space between new rect from (0, 0) to the partition rect boundary
    // 3. randomize position offset based on threshold determined in step 2
    std::function<void(/*const*/ partition&)> carve_room = [&](/*const*/ partition &part) {
        if (part.first && part.second) { // ignore non-leaf partitions
            carve_room(*part.first);
            carve_room(*part.second);
            return;
        }

        std::uniform_int_distribution<uint32_t> aDist(MIN_ROOM_LENGTH, part.size.x - 2u * PADDING);
        std::uniform_int_distribution<uint32_t> bDist(MIN_ROOM_LENGTH, part.size.y - 2u * PADDING);

        LOG(DEBUG) << "a: [" << MIN_ROOM_LENGTH << "; " << part.size.x - 2u * PADDING << "]"
        << "b: " << "[" << MIN_ROOM_LENGTH << "; " << part.size.y - 2u * PADDING << "]";

        uint32_t a { 0u };
        uint32_t b { 0u };
        do {
            a = aDist(rng);
            b = bDist(rng);
        } while (
            false // std::max(a, b) / std::min(a, b) < 3 // TODO sensible ratio
        );

        LOG(DEBUG) << "OLD: " << part.size.x << "x" << part.size.y << "\tNEW: "
                              << a << "x" << b;

        std::uniform_int_distribution<uint32_t> mxDist(PADDING, part.size.x - (a + PADDING));
        std::uniform_int_distribution<uint32_t> myDist(PADDING, part.size.y - (b + PADDING));

        part.size.x = a;
        part.size.y = b;

        part.pos.x += mxDist(rng);
        part.pos.y += myDist(rng);

        /*
        for (uint32_t y = topLeft.y; y <= bottomRight.y; y++) {
            for (uint32_t x = topLeft.x; x <= bottomRight.x; x++) {
                if (x >= part.pos.x + part.size.x || y >= part.pos.y + part.size.y) {
                    continue; // do not place on the partition boundary
                }
                auto o = new Object(); // TODO: refactor Layer data handling
                o->isVoid = false;
                layer.put(Vector2i(x, y), o);
            }
        }
        */
    };
    carve_room(test_partition);
}
