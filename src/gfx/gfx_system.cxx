#include "gfx_system.hxx"

j_gfx_system::j_gfx_system(const j_window *w) {
    window_ = w;

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    gl_context = SDL_GL_CreateContext(w->handle());

    if (gl_context == nullptr) {
        LOG(ERROR) << "Could not initialize opengl";
        throw;
    }

    SDL_GL_SetSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        LOG(ERROR) << "Could not initialize glew";
        throw;
    }

    renderer_->bind(window_);

    const auto s { window_->size() };

    display_.resize({ s.width / 8, s.height / 14 });
}

j_gfx_system::~j_gfx_system() {
    if (gl_context != nullptr) {
        SDL_GL_DeleteContext(gl_context);
    }
}

void j_gfx_system::temp__render() {
    std::stringstream ss;

    ss.precision(3);

    ss << "MSPF: " << fps_.mspf() << " (" << fps_.get_fps() << " FPS)";

    fps_.pre_render();

    j_rect_options fancy_rect_opt, simple_rect_opt, filled_rect;

    const auto dim { display_.dimensions() };

    fancy_rect_opt.corner_glyphs = j_rect_corners<uint32_t>(704, 705, 707, 706);
    fancy_rect_opt.border_glyphs = j_rect<uint32_t>(702, 703, 702, 703);
    fancy_rect_opt.color = { 128, 128, 128, 255 };
    fancy_rect_opt.span = j_rect<uint32_t>(0, dim.width - 1 , dim.height - 1, 0);

    display_.rectangle(fancy_rect_opt);

    simple_rect_opt.color = { 0, 255, 0, 255 };
    simple_rect_opt.span = j_rect<uint32_t>(2, dim.width - 3, dim.height - 3, 2);

    display_.rectangle(simple_rect_opt);

    filled_rect.span = j_rect<uint32_t>(3, dim.width - 4, dim.height - 4, 3);
    filled_rect.color = { 255, 0, 0, 255 };
    filled_rect.fill_color = { 50, 0, 0, 255 };

    display_.rectangle(filled_rect);

    j_text_options text_options;
    text_options.boundary.top = 0;
    text_options.boundary.left = 2;
    display_.text(ss.str(), text_options);

    renderer_->render(display_);
}
