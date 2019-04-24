#include "stdafx.h"

RoomGen::RoomGen() {
}

void RoomGen::generate(Layer& layer) {
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<uint32_t> roomCountRng(1, 11);

    const auto roomCount = roomCountRng(rng);

    LOG(INFO)
        << "Will generate " << roomCount << " rooms. "
        << "Interval: [" << roomCountRng.min() << "; " << roomCountRng.max() << ")\n";

    auto dimensions = layer.getDimensions();

    // generate rooms rectangles

    std::uniform_int_distribution<uint32_t> xStartRng(0, dimensions.x - 1);
    std::uniform_int_distribution<uint32_t> yStartRng(0, dimensions.y - 1);
    std::uniform_int_distribution<uint32_t> roomWidthRng(3, 16);
    std::uniform_int_distribution<uint32_t> roomHeightRng(3, 8);

    std::vector<Rect<uint32_t>> roomRects(roomCount);

    for (size_t i=0; i<roomCount; i++) {
        const auto xStart = xStartRng(rng);
        const auto yStart = yStartRng(rng);

        roomRects[i] = {
            xStart,
            yStart,
            std::min(dimensions.x - 1, xStart + roomWidthRng(rng)),
            std::min(dimensions.y - 1, yStart + roomHeightRng(rng)),
        };

        LOG(INFO)
            << "R" << i+1 << ": "
            << roomRects[i].x1 << ", " << roomRects[i].y1 << " to "
            << roomRects[i].x2 << ", " << roomRects[i].y2 << "\n";
    }

    // place room walls in layer
    // x1, y1 → x2, y1
    // x2, y1 → x2, y2
    // x2, y2 → x1, y2
    // x1, y2 → x1, y1
    // HIGHLY INEFFICIENT
    for (size_t i=0; i<roomCount; i++) {
        const auto r = roomRects[i];

        for (auto x = r.x1; x < r.x2; x++) {
            // FIXME: UNSAFE
            auto o = new Object();
            o->isSolid = true;
            layer.put(Vector2<uint32_t>(x, r.y1), o);
        }
        for (auto y = r.y1; y < r.y2; y++) {
            // FIXME: UNSAFE
            auto o = new Object();
            o->isSolid = true;
            layer.put(Vector2<uint32_t>(r.x2, y), o);
        }
        for (auto x = r.x2; x > r.x1; x--) {
            // FIXME: UNSAFE
            auto o = new Object();
            o->isSolid = true;
            layer.put(Vector2<uint32_t>(x, r.y2), o);
        }
        for (auto y = r.y2; y > r.y1; y--) {
            // FIXME: UNSAFE
            auto o = new Object();
            o->isSolid = true;
            layer.put(Vector2<uint32_t>(r.x1, y), o);
        }
    }
}
