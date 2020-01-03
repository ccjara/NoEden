#ifndef JARALYN_GAME_HXX
#define JARALYN_GAME_HXX

#include "ide_pch.hxx"

#include "frame_buffer.hxx"
#include "fps_provider.hxx"
#include "gen/room_gen.hxx"
#include "text_tester.hxx"
#include "renderer.hxx"
#include "layer.hxx"

class game {
private:
    int32_t mouse_x { 0 };
    int32_t mouse_y { 0 };

    fps_provider fps;

    entt::sigh<void(const SDL_Event&)> sdl_event_signal;
    entt::sink<void(const SDL_Event&)> sdl_event_sink;

    std::unique_ptr<text_tester> test;
    std::unique_ptr<layer> layer_ptr;

    std::unique_ptr<frame_buffer> fb;

    room_gen gen;
    room_gen_config conf;

    bool world_needs_update { true };
    inline void render_world(const renderer& rnd);
public:
    game();
    ~game() noexcept;

    void setup();

    void handle_sdl_event(const SDL_Event& e);
    void render(renderer& rnd);
};

#endif
