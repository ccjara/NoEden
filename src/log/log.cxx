#include "log.hxx"
#include "memory_sink.hxx"

Log::LogPtr Log::log_;
Log::LogStore Log::logs_;

void Log::startup() {
    log_ = spdlog::create<MemorySink>("Core");
    log_->set_level(spdlog::level::debug);
    log_->set_pattern("[%H:%M:%S][%^%L%$] %v");
}
