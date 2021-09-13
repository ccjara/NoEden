#ifndef JARALYN_LOG_HXX
#define JARALYN_LOG_HXX

#include <spdlog/sinks/base_sink.h>
#include "memory_sink.hxx"

struct LogEntry {
    enum class LogLevel {
        Debug = spdlog::level::debug,
        Info = spdlog::level::info,
        Warn = spdlog::level::warn,
        Error = spdlog::level::err,
    };
    LogLevel level;
    spdlog::log_clock::time_point time_point;
    std::string time_point_formatted;
    std::string message;
};

class Log {
    friend class LogXray;
    friend class MemorySink;
    using LogPtr = std::shared_ptr<spdlog::logger>;
    using LogStore = std::vector<LogEntry>;
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
    static LogStore logs_;
};

#endif
