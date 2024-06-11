#ifndef NOEDEN_PROFILER_HXX
#define NOEDEN_PROFILER_HXX

#include "perf_timer.hxx"

class Profiler {
public:
    static PerfTimer& timer(const char* name);

    static const std::unordered_map<const char*, PerfTimer>& timers();

    static void start_frame();

    static void stop_frame();

    static i32 get_fps();
private:
    constexpr static i32 frame_count_ = 60;

    static inline i32 current_frame_ = 0;

    static inline std::vector<double> frame_times_ = std::vector(frame_count_, 0.0);

    static inline std::unordered_map<const char*, PerfTimer> timers_;
};

#endif
