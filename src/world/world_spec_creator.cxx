#include "world/world_spec_creator.hxx"
#include "world/world_spec.hxx"
#include "framework/noise_generator.hxx"

std::unique_ptr<WorldSpec> WorldSpecCreator::create_world_spec(const CreateWorldSpecOptions& options) {
    auto spec = std::make_unique<WorldSpec>();
    spec->version_ = 1;
    spec->seed_ = options.seed;
    spec->chunks_x_ = options.chunks_x;
    spec->chunks_z_ = options.chunks_z;

    return spec;
}
