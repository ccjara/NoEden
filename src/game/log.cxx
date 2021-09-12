#include "log.hxx"

Log::LogPtr Log::log_;

void Log::startup() {
    log_ = spdlog::stdout_color_mt("Core");
    log_->set_pattern("[%H:%M:%S][%^%L%$] %v");
}
