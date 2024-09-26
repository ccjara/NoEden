#pragma once

#ifdef NOEDEN_LOGGING
    constexpr std::string_view log_fname_(std::string_view path) {
        size_t pos = path.find_last_of("/\\");
        return (pos == std::string_view::npos) ? path : path.substr(pos + 1);
    }

    #define LOG_DEBUG(message, ...) spdlog::debug("[{}:{}] {}", log_fname_(__FILE__), __LINE__, fmt::format(FMT_STRING(message), ##__VA_ARGS__))
    #define LOG_INFO(message, ...) spdlog::info("[{}:{}] {}", log_fname_(__FILE__), __LINE__, fmt::format(FMT_STRING(message), ##__VA_ARGS__))
    #define LOG_WARN(message, ...) spdlog::warn("[{}:{}] {}", log_fname_(__FILE__), __LINE__, fmt::format(FMT_STRING(message), ##__VA_ARGS__))
    #define LOG_ERROR(message, ...) spdlog::error("[{}:{}] {}", log_fname_(__FILE__), __LINE__, fmt::format(FMT_STRING(message), ##__VA_ARGS__))
#else
    #define LOG_DEBUG(message, ...)
    #define LOG_INFO(message, ...)
    #define LOG_WARN(message, ...)
    #define LOG_ERROR(message, ...)
#endif
