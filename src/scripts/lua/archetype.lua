ComponentType = {
    Render = 3,
}

function on_load()
    catalog:create_archetype({
        name = "TROLL",
        speed = 75,
        components = {
            {
                type = ComponentType.Render,
                glyph = 84, -- 'T'
            },
        },
    });
    catalog:create_archetype({
        name = "DWARF",
        speed = 100,
        components = {
            {
                type = ComponentType.Render,
                glyph = 64, -- '@'
            },
        },
    });
end

function on_unload()

end