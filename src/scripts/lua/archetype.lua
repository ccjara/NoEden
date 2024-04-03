function on_load()
    catalog:create_archetype({
        name = "TROLL",
        speed = 75,
        vision_radius = 15,
        components = {
            {
                type = ComponentType.Render,
                glyph = 84, -- 'T',
                color = 0xFF0000,
            },
            {
                type = ComponentType.Behavior,
                root = {
                    type = AiNodeType.PrioritySelector,
                    priority = 0,
                    children = {
                        {
                            type = AiNodeType.ClosestEntity,
                            found_target_key = "walk_target",
                        },
                        {
                            type = AiNodeType.Walk,
                            walk_target_key = "walk_target",
                        },
                    },
                },
            },
        },
    });
    catalog:create_archetype({
        name = "DWARF",
        speed = 100,
        vision_radius = 10,
        components = {
            {
                type = ComponentType.Render,
                glyph = 64, -- '@'
                color = 0xFFFFFF,
            },
        },
    });
end

function on_unload()
    catalog:clear_archetypes();
end