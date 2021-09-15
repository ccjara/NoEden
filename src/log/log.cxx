#include "log.hxx"

Log::LogPtr Log::log_;
Log::LogStore Log::logs_;
LogLevel Log::level_ = LogLevel::Debug;
u16 Log::max_entries_ { 1000U };

void Log::startup() {
    auto mem_sink = std::make_shared<MemorySink>();
    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        "logs/core.log",
        true
    );
    log_ = std::make_unique<spdlog::logger>("Core");
    log_->sinks().push_back(mem_sink);
    log_->sinks().push_back(file_sink);
    file_sink->set_pattern("[%H:%M:%S][%L] %v");

    set_level(LogLevel::Info);
}

void Log::set_capacity(u16 new_capacity) {
    assert(new_capacity > 0);
    logs_.clear();
    max_entries_ = new_capacity;
}

void Log::set_level(LogLevel level) {
    level_ = level;
    auto target_level { spdlog::level::debug };
    switch (level) {
        case LogLevel::Info:
            target_level = spdlog::level::info;
            break;
        case LogLevel::Warn:
            target_level = spdlog::level::warn;
            break;
        case LogLevel::Error:
            target_level = spdlog::level::err;
            break;
        case LogLevel::Debug:
        default:
            break;
    }
    for (auto& sink : log_->sinks()) {
        sink->set_level(target_level);
    }
}
