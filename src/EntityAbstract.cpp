#include "stdafx.h"

EntityAbstract::EntityAbstract() :
    active{true},
    id{INVALID_ID} {
}

const Id EntityAbstract::getId() const noexcept {
    return id;
}

void EntityAbstract::activate() noexcept {
    active = true;
}

void EntityAbstract::deactivate() noexcept {
    active = false;
}

bool EntityAbstract::isActive() const noexcept {
    return active;
}

bool EntityAbstract::operator==(const EntityAbstract& e) const {
    return id == e.id;
}

bool EntityAbstract::operator!=(const EntityAbstract& e) const {
    return id != e.id;
}

bool EntityAbstract::operator==(const EntityAbstract* e) const {
    return id == e->id;
}

bool EntityAbstract::operator!=(const EntityAbstract* e) const {
    return id != e->id;
}
