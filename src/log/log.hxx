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
    using LogStore = std::deque<LogEntry>;
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
    /**
     * @brief Library logger this class wraps.
     */
    static LogPtr log_;

    /**
     * @brief Stores logs in a FIFO-like container with random access.
     *
     * The store is currently implemented as a deque.
     */
    static LogStore logs_;

    /**
     * @brief Maximum amount of log entries before evicting the next front entry.
     */
    static u16 max_entries_;

    /**
     * @brief Clears and resizes the log store to the given size.
     *
     * Accessed from the LogXray class.
     */
    static void set_capacity(u16 new_capacity);
};

#endif
