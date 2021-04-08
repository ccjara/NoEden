#ifndef JARALYN_XRAY_INTERFACE_HXX
#define JARALYN_XRAY_INTERFACE_HXX

class j_xray_interface {
public:
    /**
     * @brief Called by the xray_system on each update
     */
    virtual void update() = 0;

    virtual ~j_xray_interface() = default;
};

#endif
