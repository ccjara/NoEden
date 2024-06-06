#ifndef NOEDEN_LOG_HXX
#define NOEDEN_LOG_HXX

enum class LogLevel {
    Debug,
    Info,
    Warn,
    Error,
};

struct LogEntry {
    LogLevel level;
    std::time_t time_point;
    std::string message;
};

class Log {
    friend class LogXray;
    friend class MemorySink;
    friend class Game;
    using LogStore = std::deque<LogEntry>;
public:
    template<typename... Args>
    static inline void debug(std::string_view fmt, Args&&... args) {
        log(LogLevel::Debug, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static inline void info(std::string_view fmt, Args&&... args) {
        log(LogLevel::Info, fmt, std::forward<Args>(args)...);
    }
    template<typename... Args>
    static inline void warn(std::string_view fmt, Args&&... args) {
        log(LogLevel::Warn, fmt, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static inline void error(std::string_view fmt, Args&&... args) {
        log(LogLevel::Error, fmt, std::forward<Args>(args)...);
    }

    /**
     * @brief Logs a message on the given level
     */
    template<typename... Args>
    static inline void log(LogLevel level, std::string_view fmt, Args&&... args) {
        if (logs_.size() == Log::max_entries_) {
            logs_.pop_front();
        }
        logs_.emplace_back(LogEntry {
            level,
            std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
            fmt::vformat(fmt, fmt::make_format_args(args...))
        });
    }

    /**
     * @brief Sets the current log level. Existing logs will be unaffected.
     */
    static void set_level(LogLevel level);
private:
    static void init();

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
