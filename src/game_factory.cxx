#include "game_factory.hxx"

j_game* game { nullptr };

void test_arena();

void j_game_factory::run() {
    game = new j_game();
    if (!game) {
        LOG(ERROR) << "Could not allocate game memory";
        std::abort();
    }

    game->env_->start();
    game->systems_->emplace<j_input_system>();
    game->systems_->emplace<j_player_system>();
    game->systems_->emplace<j_unit_system>();
    game->systems_->emplace<j_hud_system>();
    game->systems_->emplace<j_gfx_system>(&game->env_->window());
    // FIXME: emit default root_config before the script system is loaded so
    //        the gfx system can startup independently...
    game->systems_->emplace<j_script_system>();

    test_arena();

    game->run();

    delete game;
}

void test_arena() {
    auto entities { game->entities() };

    auto dwarf = entities->create();
    entities->assign<jc_position>(dwarf, 0, 1);
    entities->assign<jc_renderable>(dwarf, jc_renderable { '@', j_color::white() });
    entities->assign<jc_attribute_bearing>(dwarf);
    auto& inventory { entities->assign<jc_inventory>(dwarf) };

    auto troll = entities->create();
    entities->assign<jc_position>(troll, 5, 5);
    entities->assign<jc_renderable>(troll, jc_renderable { 'T', j_color::red() });
    entities->assign<jc_attribute_bearing>(troll);

    auto item { entities->create() };
    entities->assign<jc_position>(item, 2, 3);
    entities->assign<jc_renderable>(item, jc_renderable { '$', j_color::yellow() });
    entities->assign<jc_object_descriptor>(item, "Gold coin");
    entities->assign<jc_item>(item);

    item = entities->create();
    entities->assign<jc_position>(item, 8, 4);
    entities->assign<jc_renderable>(item, jc_renderable { '$', j_color(0x888888) });
    entities->assign<jc_object_descriptor>(item, "Silver coin");
    entities->assign<jc_item>(item);

    item = entities->create();
    entities->assign<jc_position>(item, 6, 1);
    entities->assign<jc_renderable>(item, jc_renderable { '$', j_color(0x875630) });
    entities->assign<jc_object_descriptor>(item, "Copper coin");
    entities->assign<jc_item>(item);

    game->events()->trigger<j_player_control_event>(dwarf);
}
