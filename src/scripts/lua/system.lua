gfx = {
    scaling = 1,
    font_texture_path = "font.bmp",
    glyph_size = { width = 8, height = 14 },
}

function on_load()
    env:configure(gfx);
end
