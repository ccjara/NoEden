#ifndef JARALYN_RENDER_COMPONENT_HXX
#define JARALYN_RENDER_COMPONENT_HXX

#include "component.hxx"
#include "../display_info.hxx"

class Render : public Component {
public:
    Render();

    static constinit const ComponentType static_type = ComponentType::Render;

    const DisplayInfo& display_info() const;

    DisplayInfo& display_info();

    virtual std::unique_ptr<Component> clone() const override;
private:
    DisplayInfo display_info_;
};

#endif
