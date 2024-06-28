#ifndef NOEDEN_XRAY_HXX
#define NOEDEN_XRAY_HXX

class Xray {
public:
    virtual void render() = 0;

    virtual bool initialize() {
        return true;
    }

    virtual void shutdown() {
    }

    virtual ~Xray() = default;
protected:
    friend class XrayManager;
    ServiceLocator* svc_ = nullptr;
    EventManager* events_ = nullptr;
};

#endif
