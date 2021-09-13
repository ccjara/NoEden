#ifndef JARALYN_MEMORY_SINK_HXX
#define JARALYN_MEMORY_SINK_HXX

#include <spdlog/sinks/base_sink.h>

class MemorySink : public spdlog::sinks::base_sink<std::mutex> {
protected:
    void sink_it_(const spdlog::details::log_msg& msg) override;

    void flush_() override;
};

#endif
