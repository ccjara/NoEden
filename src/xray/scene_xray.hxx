#ifndef NOEDEN_SCENE_XRAY_HXX
#define NOEDEN_SCENE_XRAY_HXX

#include "xray/xray_interface.hxx"
#include "config/config.hxx"
#include "tile/tile.hxx"

class Entity;
class EntityManager;
class TileManager;
class IInputReader;

class ConfigUpdatedEvent;
class MouseDownEvent;

class SceneXray : public IXray {
public:
    explicit SceneXray(
        EntityManager* entity_manager,
        TileManager* tile_manager,
        Events* events,
        IInputReader* input,
        Translator* translator
    );
    
    void update() override;
private:
    void entity_panel(std::optional<Id> entity_id);
    void entity_glyph(Entity* entity);

    void entity_window();
    void tile_window();

    bool on_mouse_down(MouseDownEvent& e);
    bool on_config_updated(ConfigUpdatedEvent& e);

    Config config_;

    EntityManager* entity_manager_ = nullptr;
    TileManager* tile_manager_ = nullptr;
    Events* events_ = nullptr;
    IInputReader* input_ = nullptr;
    Translator* translator_ = nullptr;
};

#endif
