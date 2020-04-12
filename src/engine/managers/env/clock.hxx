#ifndef JARALYN_CLOCK_HXX
#define JARALYN_CLOCK_HXX

class clock_c {
private:
    std::chrono::steady_clock::duration elapsed_;
    std::chrono::steady_clock::time_point last_;
public:
    [[nodiscard]] const std::chrono::steady_clock::duration& elapsed() const noexcept;

    void start() noexcept;

    void tick() noexcept;
};

#endif
