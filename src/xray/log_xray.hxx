#ifndef JARALYN_LOG_XRAY_HXX
#define JARALYN_LOG_XRAY_HXX

#include "xray_interface.hxx"

class LogXray : public IXray {
public:
    void update() override;
private:
    struct LogFilter {
        Log::LogStore results;
        bool active { false };
    } filter_;
};

#endif
