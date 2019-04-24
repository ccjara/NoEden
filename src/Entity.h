#ifndef __ENTITY_H__
#define __ENTITY_H__

template<class T>
class Entity : public EntityAbstract {
public:
    static const Id typeId;

    virtual const Id getTypeId() const override {
        return typeId;
    };
};

// auto generate staticId for each EntityAbstract derivative
template<class E>
const Id Entity<E>::typeId { StaticId<EntityAbstract>::assign<E>() };

#endif
