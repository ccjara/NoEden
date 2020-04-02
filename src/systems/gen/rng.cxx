#include "rng.hxx"

rng_t& get_rng() {
    thread_local static std::random_device rd;
    thread_local static rng_t mt { rd() };
    return mt;
}
