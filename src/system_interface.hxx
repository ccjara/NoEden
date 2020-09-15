#ifndef JARALYN_SYSTEM_INTERFACE_HXX
#define JARALYN_SYSTEM_INTERFACE_HXX

#include "identity.hxx"

/**
 * @brief Public interface used to access an arbitrary game system
 */
class j_system_interface {
public:
    virtual ~j_system_interface() = default;

    /**
     * @brief Called as soon as the system was loaded.
     *
     * All inherited dependencies are expected to be injected at this point.
     */
    virtual void on_load() = 0;

    /**
     * @brief Returns the type id of the implementing system
     */
    [[nodiscard]] virtual inline const j_static_id_t id() const = 0;

    /**
     * @brief Called every frame tick
     */
    virtual void update(uint32_t delta_time) = 0;
};

#endif
