#ifndef JARALYN_RENDER_COMPONENT_HXX
#define JARALYN_RENDER_COMPONENT_HXX

#include "generic_component.hxx"
#include "../display_info.hxx"

class Render : public GenericComponent<Render, ComponentType::Render> {
public:
    const DisplayInfo& display_info() const;

    DisplayInfo& display_info();
private:
    DisplayInfo display_info_;
};

#endif
