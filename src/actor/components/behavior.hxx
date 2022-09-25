#ifndef JARALYN_BEHAVIOR_HXX
#define JARALYN_BEHAVIOR_HXX

#include "component.hxx"

class Behavior : public Component {
public:
    Behavior();

    static constinit const ComponentType static_type = ComponentType::Behavior;

    virtual std::unique_ptr<Component> clone() const override;
protected:
};

#endif