#include "gfx_system.hxx"

void j_gfx_system::on_load() {
    window_ = &game->env().window();

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    gl_context_ = SDL_GL_CreateContext(*window_);

    if (gl_context_ == nullptr) {
        LOG(ERROR) << "Could not initialize opengl";
        std::abort();
    }
    SDL_GL_SetSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        LOG(ERROR) << "Could not initialize glew";
        std::abort();
    }

    renderer_.set_context(gl_context_);
    renderer_.set_viewport(window_->size());

    dispatcher_->sink<j_resize_event>().connect<&j_gfx_system::on_resize>(this);
    dispatcher_->sink<j_root_config_updated_event>().connect<&j_gfx_system::on_root_config_updated>(this);

    ui_ = std::make_unique<j_ui_renderer>(&display_, game->systems()->get<j_hud_system>());
    world_ = std::make_unique<j_world_renderer>(&display_);
    assert(ui_);

    configure(game->env().config());
}

void j_gfx_system::on_unload() {
    if (gl_context_ != nullptr) {
        SDL_GL_DeleteContext(gl_context_);
    }
}

void j_gfx_system::update(uint32_t delta_time) {
    fps_.pre_render();
    display_.reset();

    world_->draw();
    ui_->draw();

    renderer_.render(display_);

    dispatcher_->trigger<j_post_render_event>();

    SDL_GL_SwapWindow(*window_);
}

void j_gfx_system::on_resize(const j_resize_event& e) {
    renderer_.set_viewport(e.size);
    adjust_display();
}

void j_gfx_system::configure(const j_root_config& cfg) {
    cfg_ = cfg;

    renderer_.set_font(load_text_texture(cfg_.font_texture_path));
    renderer_.set_glyph_size(cfg_.glyph_size);
    renderer_.set_scaling(cfg_.scaling);
    adjust_display();
}

void j_gfx_system::on_root_config_updated(const j_root_config_updated_event& e) {
    configure(e.next);
}

j_texture j_gfx_system::load_text_texture(const fs::path& path) const {
    j_texture tex;
    const auto path_str { path.u8string() };
    if (!fs::exists(path)) {
        LOG(ERROR) << "Could not read text font at path " << path_str;
        std::abort();
    }
    tex.load(path_str);
    assert(tex.is_loaded());
    return tex;
}

j_renderer& j_gfx_system::renderer() {
    return renderer_;
}

j_display& j_gfx_system::display() {
    return display_;
}

void j_gfx_system::adjust_display() {
    // calculate resolution
    const auto scaled_size { window_->size() / cfg_.scaling };
    // calculate how many cells will fit on the screen given that resolution
    const j_vec2<uint32_t> display_size {
        scaled_size.x / cfg_.glyph_size.x,
        scaled_size.y / cfg_.glyph_size.y
    };
    // resize and notify
    display_.resize(display_size);
    dispatcher_->trigger<j_display_resized_event>(display_size);
}

SDL_GLContext j_gfx_system::gl_context() {
    return gl_context_;
}
