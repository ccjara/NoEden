#ifndef __ENTITY_ABSTRACT_H__
#define __ENTITY_ABSTRACT_H__

class EntityAbstract {
    friend class Entities;
private:
    // TODO: "Components" ptr here @friend Entities
protected:
    Id id; // @friend Entities
    bool active;
public:
    EntityAbstract();

    const Id getId() const noexcept;

    void activate() noexcept;
    void deactivate() noexcept;
    bool isActive() const noexcept;

    virtual const Id getTypeId() const = 0;

    bool operator==(const EntityAbstract& e) const;
    bool operator==(const EntityAbstract* e) const;
    bool operator!=(const EntityAbstract& e) const;
    bool operator!=(const EntityAbstract* e) const;
};

#endif
