#include "memory_sink.hxx"

void MemorySink::sink_it_(const spdlog::details::log_msg& msg) {
    Log::logs_.push_back(LogEntry {
        static_cast<LogEntry::LogLevel>(msg.level),
        msg.time,
        fmt::format("{:%H:%M:%S}", fmt::localtime(msg.time)),
        std::string(msg.payload.data(), msg.payload.size())
    });
}

void MemorySink::flush_() {
}
