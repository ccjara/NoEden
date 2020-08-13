#ifndef JARALYN_CLOCK_HXX
#define JARALYN_CLOCK_HXX

namespace {
    using clock = std::chrono::high_resolution_clock;

    constexpr uint32_t MAX_UPDATES { 10 }; // game updates per frame
    constexpr auto UPDATE_DURATION { std::chrono::milliseconds(12) };
    constexpr auto MAX_DELTA_DURATION { MAX_UPDATES * UPDATE_DURATION };
}

/**
 * @brief Game clock, expected to be ticked on each frame.
 */
class j_clock {
private:
    clock::duration real_time_delta_;
    clock::duration last_update_duration_;
    clock::time_point last_real_time_;
    clock::time_point present_;
    clock::time_point future_;
public:
    /**
     * @brief Advances the game clock
     *
     * Calls {@param f} on each tick. The amount of ticks depends
     * on how much real time has elapsed since the last clock update.
     */
    template<typename callback>
    constexpr inline void tick(callback f) {
        const auto now_before_update { clock::now() };

        real_time_delta_ = now_before_update - last_real_time_;
        if (real_time_delta_ > MAX_DELTA_DURATION) { // avoid death spiral
            real_time_delta_ = MAX_DELTA_DURATION;
        }
        future_ += real_time_delta_;

        while (future_ >= present_) {
            f();
            present_ += UPDATE_DURATION;
        }
        const auto now_after_update { clock::now() };

        last_update_duration_ = now_after_update - now_before_update;

        last_real_time_ = now_after_update;
    }
};

#endif
