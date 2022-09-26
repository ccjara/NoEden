ComponentType = {
    Skills = 1,
    Behavior = 2,
    Render = 3,
}

BehaviorNodeType = {
    PrioritySelector = 1,
    Walk = 1000,
}

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
                type = ComponentType.Behavior,
                root = {
                    type = BehaviorNodeType.PrioritySelector,
                    priority = 0,
                    children = {
                        {
                            type = BehaviorNodeType.Walk,
                        },
                    },
                },
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
                color = 0xFFFFFF,
            },
        },
    });
end

function on_unload()
    catalog:clear_archetypes();
end