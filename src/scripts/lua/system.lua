function on_run()
    configure();
    initialize_archetypes();
end

function configure()
    config:configure({
        scaling = 2,
        font_texture_path = "font.png",
        glyph_size = { width = 8, height = 14 },
    });

    realm:on_load(
        function(realm)
            log:debug("[system.lua]: Realm loaded: " .. realm);
            run_realm_scripts(realm);
        end
    );

    run_realm_scripts(realm:current());
end

function run_realm_scripts(realm)
    if realm == RealmType.MainMenu then
        script:run("main_menu");
    end
end

function initialize_archetypes()
    catalog:clear_archetypes();

    catalog:create_archetype({
        name = "TROLL",
        speed = 0.75,
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
                type = ComponentType.Life,
            },
            {
                type = ComponentType.Behavior,
                root = {
                    type = AiNodeType.Condition,
                    condition = AiConditionType.IsAlive,
                    child = {
                        type = AiNodeType.Sequence,
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
        },
    });
    catalog:create_archetype({
        name = "HUMAN",
        speed = 1,
        components = {
            {
                type = ComponentType.Render,
                glyph = 64, -- '@'
                color = 0xFFFFFF,
            },
            {
                type = ComponentType.Life,
            },
            {
                type = ComponentType.Vision,
                radius = 4,
            },
        },
    });
end
