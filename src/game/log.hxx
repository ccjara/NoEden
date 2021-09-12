#ifndef JARALYN_LOG_HXX
#define JARALYN_LOG_HXX

#include <spdlog/sinks/stdout_color_sinks.h>

class Log {
    using LogPtr = std::shared_ptr<spdlog::logger>;
public:
    static void startup();

    template<typename... Args>
    inline static void debug(Args&&... args) {
        log_->debug(std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline static void info(Args&&... args) {
        log_->info(std::forward<Args>(args)...);
    }
    template<typename... Args>
    inline static void warn(Args&&... args) {
        log_->warn(std::forward<Args>(args)...);
    }

    template<typename... Args>
    inline static void error(Args&&... args) {
        log_->error(std::forward<Args>(args)...);
    }
private:
    static LogPtr log_;
};

#endif
