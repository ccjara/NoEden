#ifndef JARALYN_CHANGE_CONTROLLER_HXX
#define JARALYN_CHANGE_CONTROLLER_HXX

#include "change_notification.hxx"

class change_controller {
public:
    virtual void notify(change_notification&& n) = 0;
    virtual ~change_controller() = default;
};

#endif
