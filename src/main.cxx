#include "game/game.hxx"

static_assert(!LUABRIDGE_HAS_EXCEPTIONS);

void initialize_logger();

int main() {
    initialize_logger();

    return Game::start() ? EXIT_SUCCESS : EXIT_FAILURE;
}


void spdlog_error_handler(const std::string& msg) {
    const auto error_message = fmt::format("Fatal spdlog error: {}\n", msg.c_str());
    std::fputs(error_message.c_str(), stderr);
}

inline void initialize_logger() {
    spdlog::set_error_handler(spdlog_error_handler);

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::info);
    console_sink->set_pattern("[%H:%M:%S:%e][%^%l%$] %v");

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/core.log", true);
    file_sink->set_level(spdlog::level::debug);
    file_sink->set_pattern("[%Y-%m-%d %H:%M:%S:%e][%l] %v");

    auto logger = std::make_shared<spdlog::logger>("multi_sink", spdlog::sinks_init_list{console_sink, file_sink});
    logger->set_level(spdlog::level::debug);
    spdlog::set_default_logger(logger);
}
