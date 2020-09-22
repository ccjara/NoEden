#ifndef JARALYN_GAME_FACTORY_HXX
#define JARALYN_GAME_FACTORY_HXX

#include "gfx/gfx_system.hxx"
#include "scripts/script_system.hxx"
#include "input/input_system.hxx"
#include "player/player_system.hxx"
#include "hud/hud_system.hxx"
#include "controller/controller_system.hxx"
#include "states/state_stack.hxx"

class j_game_factory {
public:
    static void run();
};

#endif
