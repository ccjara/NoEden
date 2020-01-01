#include "rng.hxx"

std::mt19937& get_rng() {
    thread_local static std::random_device rd;
    thread_local static std::mt19937 mt { rd() };
    return mt;
}
