#include "../../engine/framework/game_system.hxx"
#include "../../engine/framework/system_scene.hxx"
#include "../system_definition.hxx"
#include "gfx_scene.hxx"

#include "renderer.hxx"

class gfx_system : public game_system {
protected:
public:
    system_id_t id() const noexcept override {
        return static_cast<system_id_t> (system_id::gfx);
    };
};


namespace detail {
    class gfx_system_impl : public gfx_system {
    private:
        const window* window_ = nullptr;

        std::unique_ptr<renderer> renderer_ = std::make_unique<renderer>();
    public:
        explicit gfx_system_impl(const window* w);
    };
}
