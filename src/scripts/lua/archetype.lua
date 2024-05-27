function on_load()
    catalog:create_archetype({
        name = "TROLL",
        speed = 75,
        components = {
            {
                type = ComponentType.Render,
                glyph = 84, -- 'T',
                color = 0xFF0000,
            },
            {
                type = ComponentType.Vision,
                radius = 10,
            },
            {
                type = ComponentType.Behavior,
                root = {
                    type = AiNodeType.Sequence,
                    children = {
                        {
                            type = AiNodeType.ClosestEntity,
                            found_target_key = "walk_target",
                        },
                        {
                            type = AiNodeType.Walk,
                            --walk_target_key = "walk_target",
                        },
                    },
                },
            },
        },
    });
    catalog:create_archetype({
        name = "DWARF",
        speed = 100,
        vision_radius = 4,
        components = {
            {
                type = ComponentType.Render,
                glyph = 64, -- '@'
                color = 0xFFFFFF,
            },
            {
                type = ComponentType.Vision,
                radius = 4,
            },
        },
    });
end

function on_unload()
    catalog:clear_archetypes();
end