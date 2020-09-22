#ifndef JARALYN_CONTROLLER_HXX
#define JARALYN_CONTROLLER_HXX

class j_controller_system;

/**
 * @brief Extension point for all controllers
 *
 * As an entity is nothing but a simple data container, with logic being mainly
 * restricted to its housekeeping, controllers may act on entities and modify
 * their state.
 */
class j_controller {
private:
    friend class j_controller_system;
protected:
    /**
     * @brief Engine-level dependency which is injected by the controller system
     */
    entt::dispatcher* dispatcher_ { nullptr };

    /**
     * @brief Use the lifecycle method {@see on_create()} instead (do not override)
     *
     * Controllers may only be created by the {@see j_controller_system}
     */
    j_controller() = default;

    /**
     * @brief Flag indicating cleanup to the controller system
     *
     * Destroyed controllers will not have their lifecycle callbacks called
     * and are destroyed on the next housekeeping tick by the controller system.
     */
    bool destroyed_ { false };
public:
    /**
     * @brief Use the lifecycle method {@see on_destroy()} instead (do not override)
     */
    virtual ~j_controller();

    /**
     * @brief Called after the controller has been created
     *
     * As with systems, engine level dependencies have been injected at this point.
     * Override this method to attach to game events. Do not forget to detach
     * in {@see on_destroy()}.
     */
    virtual void on_create();

    /**
     * @brief Called prior to the controller's destruction
     *
     * Detach any event listeners you may have registered in {@see on_create()}
     */
    virtual void on_destroy();

    /**
     * @brief Called on each game tick as a subroutine of the controller system
     */
    virtual void on_update(uint32_t delta_time);

    /**
     * @brief Destroy a controller programmatically
     *
     * As of now a controller may opt to destroy itself.
     */
    void destroy();
};

#endif
