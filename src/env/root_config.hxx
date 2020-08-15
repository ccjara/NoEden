#ifndef JARALYN_ROOT_CONFIG_HXX
#define JARALYN_ROOT_CONFIG_HXX

/**
 * @brief Contains platform / system options for the game
 */
struct j_root_config {
    j_vec2<uint32_t> glyph_size { 8, 14 };
    uint32_t scaling { 1 };
    fs::path font_texture_path { "font.bmp" };
};

#endif
