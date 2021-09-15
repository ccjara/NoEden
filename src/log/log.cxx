#include "log.hxx"

Log::LogPtr Log::log_;
Log::LogStore Log::logs_;
LogLevel Log::level_ = LogLevel::Debug;
u16 Log::max_entries_ { 1000U };

void Log::startup() {
    log_ = spdlog::create<MemorySink>("Core");
    set_level(LogLevel::Debug);
    log_->set_pattern("[%H:%M:%S][%^%L%$] %v");
}

void Log::set_capacity(u16 new_capacity) {
    assert(new_capacity > 0);
    logs_.clear();
    max_entries_ = new_capacity;
}

void Log::set_level(LogLevel level) {
    level_ = level;
    switch (level) {
        case LogLevel::Debug:
        default:
            return spdlog::set_level(spdlog::level::debug);
        case LogLevel::Info:
            return spdlog::set_level(spdlog::level::info);
        case LogLevel::Warn:
            return spdlog::set_level(spdlog::level::warn);
        case LogLevel::Error:
            return spdlog::set_level(spdlog::level::err);
    }
}
