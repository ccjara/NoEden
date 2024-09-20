#include "world/chunk_generator.hxx"
#include "world/chunk.hxx"
#include "world/world_spec.hxx"
#include "framework/noise_generator.hxx"

std::unique_ptr<Chunk> ChunkGenerator::generate_chunk(const GenerateChunkOptions&) {
    std::unique_ptr<Chunk> chunk = std::make_unique<Chunk>();

    for (i32 x = 0; x < Chunk::CHUNK_SIDE_LENGTH; ++x) {
        for (i32 z = 0; z < Chunk::CHUNK_SIDE_LENGTH; ++z) {
            for (i32 y = 0; y < Chunk::CHUNK_DEPTH; ++y) {
                Tile& tile = chunk->tile(x, y, z);

                tile.type = TileType::Floor;
                tile.flags.set(FoV, true);
                tile.flags.set(Revealed, true);

                switch (std::rand() % 5) {
                    case 0:
                        tile.display_info.glyph = 39; // '
                        tile.display_info.color = Color::lime();
                        break;
                    case 1:
                        tile.display_info.glyph = 44; // ,
                        tile.display_info.color = Color::lime();
                        break;
                    case 2:
                        tile.display_info.glyph = 46; // .
                        tile.display_info.color = Color::green();
                        break;
                    case 3:
                        tile.display_info.glyph = 96; // `
                        tile.display_info.color = Color::green();
                        break;
                    case 4:
                        tile.display_info.glyph = '"'; // "
                        tile.display_info.color = Color::lime();
                        break;
                }

                tile.material = MaterialType::Vegetation;
                tile.state = MaterialState::Solid;
            }
        }
    }


    // place ramps
    /*
    for (i32 z = 1; z < Chunk::CHUNK_SIDE_LENGTH; ++z) {
        for (i32 x = 1; x < Chunk::CHUNK_SIDE_LENGTH; ++x) {
            const i32 height_value = chunk->height_map[x + z * Chunk::CHUNK_SIDE_LENGTH];
            const i32 height_left = chunk->height_map[(x - 1) + z * Chunk::CHUNK_SIDE_LENGTH];
            const i32 height_back = chunk->height_map[x + (z - 1) * Chunk::CHUNK_SIDE_LENGTH];

            const auto delta_left = height_value - height_left;
            const auto delta_back = height_value - height_back;

            if (delta_left == 1) { // ‗= place ramp at ‗ (we are at =)
                Tile& rampUp = chunk->tile(x - 1, height_left, z);
                rampUp.display_info.glyph = 756;
                rampUp.flags.set(TileFlags::Ramp);
                rampUp.flags.set(TileFlags::Revealed);
                rampUp.type = TileType::Floor;

                Tile& rampDown = chunk->tile(x - 1, height_value, z);
                rampDown.display_info.glyph = 758;
                rampDown.flags.set(TileFlags::Ramp);
                rampDown.flags.set(TileFlags::Revealed);
                rampDown.type = TileType::Floor;
            } else if (delta_left == -1) { // =‗ place ramp at ‗ (we are at ‗)
                Tile& rampUp = chunk->tile(x, height_value, z);
                rampUp.display_info.glyph = 756;
                rampUp.flags.set(TileFlags::Ramp);
                rampUp.flags.set(TileFlags::Revealed);
                rampUp.type = TileType::Floor;

                Tile& rampDown = chunk->tile(x, height_left, z);
                rampDown.display_info.glyph = 758;
                rampDown.flags.set(TileFlags::Ramp);
                rampDown.flags.set(TileFlags::Revealed);
                rampDown.type = TileType::Floor;
            }

            if (delta_back == 1) { // place northern ramp
                Tile& rampUp = chunk->tile(x, height_back, z - 1);
                rampUp.display_info.glyph = 756;
                rampUp.flags.set(TileFlags::Ramp);
                rampUp.flags.set(TileFlags::Revealed);
                rampUp.type = TileType::Floor;

                Tile& rampDown = chunk->tile(x, height_value, z - 1);
                rampDown.display_info.glyph = 758;
                rampDown.flags.set(TileFlags::Ramp);
                rampDown.flags.set(TileFlags::Revealed);
                rampDown.type = TileType::Floor;
            } else if (delta_back == -1) { // place southern ramp
                Tile& rampUp = chunk->tile(x, height_value, z);
                rampUp.display_info.glyph = 756;
                rampUp.flags.set(TileFlags::Ramp);
                rampUp.flags.set(TileFlags::Revealed);
                rampUp.type = TileType::Floor;

                Tile& rampDown = chunk->tile(x, height_back, z);
                rampDown.display_info.glyph = 758;
                rampDown.flags.set(TileFlags::Ramp);
                rampDown.flags.set(TileFlags::Revealed);
                rampDown.type = TileType::Floor;
            }
        }
    }
    */

    return chunk;
}
