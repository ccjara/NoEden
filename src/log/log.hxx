#ifndef NOEDEN_LOG_HXX
#define NOEDEN_LOG_HXX

#ifdef NOEDEN_LOGGING
    #define LOG_DEBUG(message, ...) Log::debug(message, std::source_location::current(), ##__VA_ARGS__)
    #define LOG_INFO(message, ...) Log::info(message, std::source_location::current(), ##__VA_ARGS__)
    #define LOG_WARN(message, ...) Log::warn(message, std::source_location::current(), ##__VA_ARGS__)
    #define LOG_ERROR(message, ...) Log::error(message, std::source_location::current(), ##__VA_ARGS__)
#else
    #define LOG_DEBUG(message, ...)
    #define LOG_INFO(message, ...)
    #define LOG_WARN(message, ...)
    #define LOG_ERROR(message, ...)
#endif

enum class LogLevel {
    Debug,
    Info,
    Warn,
    Error,
};

struct LogEntry {
    LogLevel level = LogLevel::Debug;
    std::source_location location = std::source_location::current();
    std::time_t time_point = 0;
    std::string message = "";
};

class Log {
    friend class LogXray;
    friend class MemorySink;
    friend class Game;
    using LogStore = std::deque<LogEntry>;
public:
    template<typename... Args>
    static void debug(std::string_view fmt, std::source_location location = std::source_location::current(), Args&&... args) {
        log(LogLevel::Debug, location, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void info(std::string_view fmt, std::source_location location = std::source_location::current(), Args&&... args) {
        log(LogLevel::Info, location, fmt, std::forward<Args>(args)...);
    }
    template<typename... Args>
    static void warn(std::string_view fmt, std::source_location location = std::source_location::current(), Args&&... args) {
        log(LogLevel::Warn, location, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void error(std::string_view fmt, std::source_location location = std::source_location::current(), Args&&... args) {
        log(LogLevel::Error, location, fmt, std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message on the given level
     */
    template<typename... Args>
    static void log(LogLevel level, const std::source_location& location, std::string_view fmt, Args&&... args) {
        if (logs_.size() == Log::max_entries_) {
            logs_.pop_front();
        }
        logs_.emplace_back(LogEntry {
            level,
            location,
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
            fmt::vformat(fmt, fmt::make_format_args(args...))
        });
    }

    /**
     * @brief Sets the current log level. Existing logs will be unaffected.
     */
    static void set_level(LogLevel level);
private:
    static void initialize();

    /**
     * @brief Stores logs in a FIFO-like container with random access.
     *
     * The store is currently implemented as a deque.
     */
    static inline LogStore logs_;

    /**
     * @brief Maximum amount of log entries before evicting the next front entry.
     */
    static inline u16 max_entries_ = 1000U;

    /**
     * @brief The current log level.
     */
    static inline LogLevel level_ = LogLevel::Debug;

    /**
     * @brief Clears and resizes the log store to the given size.
     *
     * Accessed from the LogXray class.
     */
    static void set_capacity(u16 new_capacity);
};

#endif
