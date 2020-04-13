#include "clock.hxx"

const std::chrono::steady_clock::duration& j_clock::elapsed() const noexcept {
    return elapsed_;
}

void j_clock::start() noexcept {
    last_ = std::chrono::high_resolution_clock::now();
}

void j_clock::tick() noexcept {
    const auto now { std::chrono::high_resolution_clock::now() };

    elapsed_ = now - last_;

    last_ = now;
}
