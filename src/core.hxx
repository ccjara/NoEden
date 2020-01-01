#ifndef JARALYN_CORE_HXX
#define JARALYN_CORE_HXX

#include "ide_pch.hxx"

#include "renderer.hxx"
#include "window.hxx"
#include "game.hxx"

class core {
private:
    std::shared_ptr<window> window_ptr;
    std::unique_ptr<renderer> renderer_ptr;
    std::unique_ptr<game> game_ptr;

    void process_events();
    void render();
    bool is_running = false;
public:
    core();
    void run();
    void shutdown() noexcept;
};

#endif
