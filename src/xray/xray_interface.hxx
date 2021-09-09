#ifndef JARALYN_XRAY_INTERFACE_HXX
#define JARALYN_XRAY_INTERFACE_HXX

class IXray {
public:
    /**
     * @brief Called by `Xray` on each update
     */
    virtual void update() = 0;

    virtual ~IXray() = default;
};

#endif
