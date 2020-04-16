#ifndef JARALYN_CLOCK_HXX
#define JARALYN_CLOCK_HXX

class j_clock {
private:
    std::chrono::high_resolution_clock::duration elapsed_;
    std::chrono::high_resolution_clock::time_point last_;
public:
    [[nodiscard]] const std::chrono::high_resolution_clock::duration& elapsed() const noexcept;

    [[nodiscard]] float_t ratio() const noexcept;

    void start() noexcept;

    void tick() noexcept;
};

#endif
