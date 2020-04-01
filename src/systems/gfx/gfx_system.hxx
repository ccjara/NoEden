#include "../game_system.hxx"

#include "renderer.hxx"

class gfx_system : public game_system {
private:
    const window* window_ = nullptr;

    std::unique_ptr<renderer> renderer_ = std::make_unique<renderer>();
public:
    void on_load(const manager_provider& managers_) override;
};
