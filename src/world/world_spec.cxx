#include "world/world_spec.hxx"

float WorldSpec::height_at(const ChunkPos&) const {
    return 0;
}

i32 WorldSpec::seed() const {
    return seed_;
}

i32 WorldSpec::version() const {
    return version_;
}

i32 WorldSpec::chunks_x() const {
    return chunks_x_;
}

i32 WorldSpec::chunks_z() const {
    return chunks_z_;
}
