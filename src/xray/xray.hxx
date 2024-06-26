#ifndef NOEDEN_XRAY_HXX
#define NOEDEN_XRAY_HXX

class Xray {
public:
    virtual void render() = 0;

    virtual ~Xray() = default;
};

#endif
