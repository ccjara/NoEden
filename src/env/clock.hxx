#ifndef JARALYN_CLOCK_HXX
#define JARALYN_CLOCK_HXX

namespace {
    using j_clock_t = std::chrono::high_resolution_clock;

    constexpr uint32_t MAX_UPDATES { 10 };
    constexpr auto UPDATE_DURATION { std::chrono::milliseconds(12) };
    constexpr auto MAX_DELTA_DURATION { MAX_UPDATES * UPDATE_DURATION };
}

class j_clock {
private:
    j_clock_t::duration real_time_delta_;
    j_clock_t::duration last_update_duration_;
    j_clock_t::time_point last_real_time_;
    j_clock_t::time_point present_;
    j_clock_t::time_point future_;
public:
    template<typename callback>
    constexpr inline void tick(callback f) {
        const auto now_before_update { j_clock_t::now() };

        real_time_delta_ = now_before_update - last_real_time_;
        if (real_time_delta_ > MAX_DELTA_DURATION) { // avoid death spiral
            real_time_delta_ = MAX_DELTA_DURATION;
        }
        future_ += real_time_delta_;

        while (future_ >= present_) {
            f();
            present_ += UPDATE_DURATION;
        }
        const auto now_after_update { j_clock_t::now() };

        last_update_duration_ = now_after_update - now_before_update;

        last_real_time_ = now_after_update;
    }
};

#endif
