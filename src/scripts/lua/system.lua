function on_run()
    configure();
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
