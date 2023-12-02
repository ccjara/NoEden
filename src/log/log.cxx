#include "log.hxx"

void Log::init() {
    set_level(LogLevel::Debug);
}

void Log::set_capacity(u16 new_capacity) {
    assert(new_capacity > 0);
    logs_.clear();
    max_entries_ = new_capacity;
}

void Log::set_level(LogLevel level) {
    level_ = level;
}
