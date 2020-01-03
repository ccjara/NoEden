#ifndef JARALYN_FPS_PROVIDER_HXX
#define JARALYN_FPS_PROVIDER_HXX

#include "ide_pch.hxx"

constexpr std::chrono::milliseconds FPS_INTERVAL { 1000 };

class fps_provider {
private:
    uint32_t fps { 0U };
    uint32_t frames { 0U };
    uint32_t prev_ticks { 0U };
public:
    void pre_render() {
        frames++;

        const auto ticks { SDL_GetTicks() };
        if (prev_ticks < ticks - FPS_INTERVAL.count()) {
            prev_ticks = ticks;
            fps = frames;
            frames = 0;
        }
    }

    [[nodiscard]] uint32_t get_fps() const noexcept {
        return fps;
    }
};

#endif
