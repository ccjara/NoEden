#ifndef JARALYN_CONTROLLER_SYSTEM_HXX
#define JARALYN_CONTROLLER_SYSTEM_HXX

#include "../game.hxx"
#include "../system.hxx"

#include "unit_controller.hxx"

/**
 * @brief Manages controllers which act on entities
 *
 * @see j_controller
 */
class j_controller_system : public j_system<j_controller_system> {
private:
    std::vector<std::unique_ptr<j_controller>> controllers_;

    /**
     * @brief Factory method which instantiates a controller
     */
    template<typename controller>
    void create() {
        j_controller* c = new controller();
        if (!c) {
            LOG(ERROR) << "Could not allocate controller memory";
            return;
        }
        controllers_.emplace_back(c);
        c->dispatcher_ = dispatcher_;
        // engine dependencies must be injected at this point
        c->on_create();
    }
public:
    void on_load() override;
    void update(uint32_t delta_time) override;
};

#endif
