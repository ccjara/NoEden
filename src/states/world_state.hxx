#ifndef JARALYN_WORLD_STATE_HXX
#define JARALYN_WORLD_STATE_HXX

#include "../game.hxx"
#include "../grid.hxx"
#include "../components/components.hxx"
#include "../components/item.hxx"
#include "../event/player_event.hxx"
#include "../hud/hud_system.hxx"
#include "state.hxx"

struct j_segment {
    bool is_void;
    int32_t glyph;
    j_color color;
};

struct j_tile {
    j_segment wall;
    j_segment floor;
    j_display_cell displayed_cell; // should later generate from setting wall and floor
};

class j_world_state : public j_state {
private:
    j_grid<j_tile> layer_;

    entt::entity dwarf_;
    entt::entity troll_;

    inline void render_hud(j_display& display);
public:
    j_world_state() : layer_(j_tile{}) {

    }

    const j_grid<j_tile>& layer() const;

    virtual void on_render(j_display& display) override;

    virtual void on_enter() override;

    virtual void on_update() override;

    virtual void on_leave() override;
};

#endif
