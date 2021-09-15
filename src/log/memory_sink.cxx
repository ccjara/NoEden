#include "memory_sink.hxx"

void MemorySink::sink_it_(const spdlog::details::log_msg& msg) {
    auto& logs { Log::logs_ };
    if (logs.size() == Log::max_entries_) {
        logs.pop_front();
    }
    logs.emplace_back(LogEntry {
        static_cast<LogLevel>(msg.level),
        msg.time,
        fmt::format("{:%H:%M:%S}", fmt::localtime(msg.time)),
        std::string(msg.payload.data(), msg.payload.size())
    });
}

void MemorySink::flush_() {
}
