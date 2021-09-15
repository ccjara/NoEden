#ifndef JARALYN_LOG_XRAY_HXX
#define JARALYN_LOG_XRAY_HXX

#include "xray_interface.hxx"
#include "xray_style.hxx"

class LogXray : public IXray {
public:
    void update() override;
private:
    struct LogFilter {
        std::string_view message_contains;
        Log::LogStore results;
    } filter_;
};

#endif
