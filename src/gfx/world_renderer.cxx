#include "world_renderer.hxx"

void j_world_renderer::draw() {
    game->entities()->view<jc_renderable, jc_position>().each([this](auto& renderable, auto& position) {
        if (!display_->in_bounds(position)) {
            return; // do not render entities outside of view
        }
        display_->put(
            j_display_cell(renderable.glyph, renderable.color),
            position,
            fast_grid_access_tag {} // bounds checked above
        );
    });
}