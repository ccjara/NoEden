#include "instrumentation/profiler.hxx"

PerfTimer& Profiler::timer(const char* name) {
    return timers_.try_emplace(name, name).first->second;
}

const std::unordered_map<const char*, PerfTimer>& Profiler::timers() {
    return timers_;
}

void Profiler::start_frame() {
    timer("Frame").start();
}

void Profiler::stop_frame() {
    auto& t = timer("Frame");
    t.stop();

    frame_times_[current_frame_ % frame_count_] = t.duration() / 1e6;
    ++current_frame_;
}

i32 Profiler::get_fps() {
    double avg = 0.0;
    for (double time : frame_times_) {
        avg += time;
    }
    avg /= frame_count_;

    if (avg == 0) {
        return 0;
    }

    return static_cast<i32>(1.0 / avg);
}
