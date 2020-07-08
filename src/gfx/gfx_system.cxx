#include "gfx_system.hxx"

j_gfx_system::j_gfx_system(const j_window* w) {
    assert(w);
    window_ = w;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    gl_context = SDL_GL_CreateContext(*w);

    if (gl_context == nullptr) {
        LOG(ERROR) << "Could not initialize opengl";
        throw;
    }

    SDL_GL_SetSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        LOG(ERROR) << "Could not initialize glew";
        throw;
    }

    const auto vp { window_->size() };
    renderer_ = std::make_unique<j_renderer>();
    renderer_->set_viewport(vp);

    display_.resize(j_size<uint32_t>{ vp.width / 8, vp.height / 14 });
}

j_gfx_system::~j_gfx_system() {
    if (gl_context != nullptr) {
        SDL_GL_DeleteContext(gl_context);
    }
}

void j_gfx_system::prepare() {
    fps_.pre_render();

    display_.reset();
}

void j_gfx_system::present() {
    renderer_->render(display_);

    SDL_GL_SwapWindow(*window_);
}

void j_gfx_system::attach(entt::dispatcher& dispatcher) {
    dispatcher.sink<j_resize_event>().connect<&j_gfx_system::on_resize_event>(this);
}

void j_gfx_system::on_resize_event(const j_resize_event& e) {
    display_.resize(j_size<uint32_t>{ e.size.width / 8, e.size.height / 14 });
    renderer_->set_viewport(e.size);
}

j_renderer& j_gfx_system::renderer() noexcept {
    return *renderer_;
}

j_display& j_gfx_system::display() noexcept {
    return display_;
}
