#ifndef JARALYN_GAME_HXX
#define JARALYN_GAME_HXX

#include "ide_pch.hxx"

#include "gen/room_gen.hxx"
#include "text_tester.hxx"
#include "renderer.hxx"
#include "layer.hxx"

class game {
private:
    entt::sigh<void(const SDL_Event&)> sdl_event_signal;
    entt::sink<void(const SDL_Event&)> sdl_event_sink;

    std::unique_ptr<text_tester> test;
    std::unique_ptr<layer> layer_ptr;

    room_gen gen;

    uint32_t max_depth;
public:
    game();
    ~game() noexcept;
    void handle_sdl_event(const SDL_Event& e);
    void render(const renderer& rnd);
};

#endif
