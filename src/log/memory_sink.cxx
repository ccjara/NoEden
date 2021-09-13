#include "memory_sink.hxx"

void MemorySink::sink_it_(const spdlog::details::log_msg& msg) {
    Log::logs_.push_back(LogEntry {
        LogEntry::LogLevel::Info,
        msg.time,
        fmt::format("{}", 999),
        std::string(msg.payload.data(), msg.payload.size())
    });
}

void MemorySink::flush_() {
}
