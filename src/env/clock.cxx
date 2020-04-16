#include "clock.hxx"

const std::chrono::high_resolution_clock::duration& j_clock::elapsed() const noexcept {
    return elapsed_;
}

float_t j_clock::ratio() const noexcept {
    using time_unit = std::chrono::microseconds;

    return static_cast<float_t>(
        std::chrono::duration_cast<time_unit>(elapsed_).count()
    ) / (time_unit::period::den / 60.0f);
}

void j_clock::start() noexcept {
    last_ = std::chrono::high_resolution_clock::now();
}

void j_clock::tick() noexcept {
    const auto now { std::chrono::high_resolution_clock::now() };

    elapsed_ = now - last_;

    last_ = now;
}
