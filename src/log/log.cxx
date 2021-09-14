#include "log.hxx"

Log::LogPtr Log::log_;
Log::LogStore Log::logs_;
u16 Log::max_entries_ { 1000U };

void Log::startup() {
    log_ = spdlog::create<MemorySink>("Core");
    log_->set_level(spdlog::level::debug);
    log_->set_pattern("[%H:%M:%S][%^%L%$] %v");
}

void Log::set_capacity(u16 new_capacity) {
    assert(new_capacity > 0);
    logs_.clear();
    max_entries_ = new_capacity;
}