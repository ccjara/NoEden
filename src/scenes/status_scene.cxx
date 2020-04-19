#include "status_scene.hxx"

j_status_scene::j_status_scene() {
    is_blocking_ = true;
    is_opaque_ = true;
}

void j_status_scene::update(j_input_state& input) {
    if (input.keyboard().consume(SDL_KeyCode::SDLK_ESCAPE)) {
        scene_writer_->unload(id());
    }
}

void j_status_scene::render(j_display& display) {
    j_text_options opt;

    opt.boundary.left = 1;
    opt.boundary.top = 1;

    display.text("You are now in the status screen", opt);
}
