#include "clock.hxx"

const std::chrono::steady_clock::duration& clock_c::elapsed() const noexcept {
    return elapsed_;
}

void clock_c::start() noexcept {
    last_ = std::chrono::high_resolution_clock::now();
}

void clock_c::tick() noexcept {
    const auto now { std::chrono::high_resolution_clock::now() };

    elapsed_ = now - last_;

    last_ = now;
}
