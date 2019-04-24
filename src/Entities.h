#ifndef __ENTITIES_H__
#define __ENTITIES_H__

// TODO: move non-template impls

class Entities {
private:
    IdGenerator idGen;
    
    // TODO: Components dependency

    class EntityHolderAbstract {
    public:
        virtual void hold(std::shared_ptr<EntityAbstract> entity) = 0;
    };

    template<class E>
    class EntityHolder : public EntityHolderAbstract {
    private:
        std::vector<std::shared_ptr<EntityAbstract>> entities;
    public:
        void hold(std::shared_ptr<EntityAbstract> entity) override {
            entities.push_back(entity);
        };
        // todo: drop
    };

    using EntityDictionary = std::unordered_map<Id, std::unique_ptr<EntityHolderAbstract>>;
    EntityDictionary entityDictionary;

    template<class E>
    EntityHolder<E>* getEntityHolder() {
        const Id typeId { E::typeId };
        auto iter { entityDictionary.find(typeId) };

        if (iter == entityDictionary.end()) {
            // SIMPLIFY
            auto holder { std::make_unique<EntityHolder<E>>() };
            auto holderRawPtr { static_cast<EntityHolder<E>*> (holder.get()) };
            entityDictionary[typeId] = std::move(holder);
            return holderRawPtr;
        }

        auto holderPtr { static_cast<EntityHolder<E>*> (iter->second.get()) };

        if (holderPtr == nullptr) {
            LOG(ERROR)
                << "Instance of EntityHolder<" << typeid(E).name() << "> is null!"
                << "Crashing due to data corruption";
            throw;
        }
        return holderPtr;
    }
public:
    // TODO: depend on Components
    Entities() {
    };

    template<class E> 
    Id create() {
        auto holder { getEntityHolder<E>() };
        auto entity { std::make_shared<E>() };
        const auto id { idGen.generate() };

        // friend access
        entity->id = id;
        // TODO: assign components pointer
        holder->hold(entity);

        return id;
    }
};

#endif
