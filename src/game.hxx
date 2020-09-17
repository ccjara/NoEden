#ifndef JARALYN_GAME_HXX
#define JARALYN_GAME_HXX

#include "systems.hxx"
#include "states/state_stack.hxx"

#include "env/env_manager.hxx"

#include "gfx/gfx_system.hxx"
#include "scripts/script_system.hxx"
#include "input/input_system.hxx"
#include "player/player_system.hxx"
#include "hud/hud_system.hxx"

class j_systems;
class j_state_stack;

class j_game {
private:
    j_state_stack state_;

    std::unique_ptr<j_env_manager> env_;
    std::unique_ptr<j_systems> systems_;
    entt::registry entities_;
    entt::dispatcher dispatcher_;
public:
    j_game();
    j_systems* systems();
    entt::registry* entities();
    entt::dispatcher* events();

    void run();
};

extern j_game* game;

#endif
