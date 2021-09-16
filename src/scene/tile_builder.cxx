#include "tile_builder.hxx"

Tile TileBuilder::wall() {
    Tile wall;
    wall.type = TileType::Wall;
    wall.display_info.glyph = '#';
    wall.display_info.color = Color::white();
    wall.solid = true;
    return wall;
}

Tile TileBuilder::floor() {
    Tile floor;
    floor.type = TileType::Floor;
    floor.display_info.glyph = '.';
    floor.display_info.color = Color::white();
    floor.solid = false;
    return floor;
}