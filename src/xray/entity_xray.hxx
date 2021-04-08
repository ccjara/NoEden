#ifndef JARALYN_ENTITY_XRAY_HXX
#define JARALYN_ENTITY_XRAY_HXX

#include "../game.hxx"
#include "xray_interface.hxx"
#include "../components/components.hxx"
#include "../components/item.hxx"

class j_entity_xray : public j_xray_interface {
public:
    void update() override;
};

#endif
