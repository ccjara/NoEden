#include "gfx_system.hxx"

j_gfx_system::j_gfx_system(j_window* window) : window_(window) {
    assert(window_);

    if (gl_context != nullptr) {
        SDL_GL_DeleteContext(gl_context);
    }
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    gl_context = SDL_GL_CreateContext(*window_);

    if (gl_context == nullptr) {
        LOG(ERROR) << "Could not initialize opengl";
        std::abort();
    }
    SDL_GL_SetSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        LOG(ERROR) << "Could not initialize glew";
        std::abort();
    }

    renderer_.set_context(gl_context);
    renderer_.set_viewport(window_->size());
}

j_gfx_system::~j_gfx_system() {
    if (gl_context != nullptr) {
        SDL_GL_DeleteContext(gl_context);
    }
}

void j_gfx_system::on_load() {
    dispatcher_->sink<j_resize_event>().connect<&j_gfx_system::on_resize>(this);
    dispatcher_->sink<j_root_config_updated_event>().connect<&j_gfx_system::on_root_config_updated>(this);
}

void j_gfx_system::update(uint32_t delta_time) {
    fps_.pre_render();
    display_.reset();

    game->entities()->view<jc_renderable, jc_position>().each([this](auto& renderable, auto& position) {
        j_vec2<uint32_t> pos {
            static_cast<uint32_t> (position.x),
            static_cast<uint32_t> (position.y)
        };

        if (position.x < 0 || position.y < 0 || !display_.in_bounds(pos)) {
            return;
        }

        display_.text(renderable.text, position, renderable.text_options);
    });

    renderer_.render(display_);

    SDL_GL_SwapWindow(*window_);
}

void j_gfx_system::on_resize(const j_resize_event& e) {
    renderer_.set_viewport(e.size);
    adjust_display();
}

void j_gfx_system::on_root_config_updated(const j_root_config_updated_event& e) {
    cfg_ = e.next;
    renderer_.set_font(load_text_texture(cfg_.font_texture_path));
    renderer_.set_glyph_size(cfg_.glyph_size);
    renderer_.set_scaling(cfg_.scaling);

    adjust_display();
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
    display_.resize({
        scaled_size.x / cfg_.glyph_size.x,
        scaled_size.y / cfg_.glyph_size.y
    });
}

