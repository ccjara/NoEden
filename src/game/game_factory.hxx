#ifndef JARALYN_GAME_FACTORY_HXX
#define JARALYN_GAME_FACTORY_HXX

#include "game.hxx"
#include "../ai/ai_walk.hxx"
#include "../scripts/api/api_registrar.hxx"
#include "../scripts/api/scene_api.hxx"
#include "../scripts/api/ui/ui_api.hxx"
#include "../xray/log_xray.hxx"
#include "../xray/scene_xray.hxx"
#include "../xray/script_xray.hxx"
#include "../xray/ui_xray.hxx"

class GameFactory {
public:
    static void run();
};

#endif
