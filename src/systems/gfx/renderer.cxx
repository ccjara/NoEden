#include "renderer.hxx"

renderer::renderer() :
    gl_context(nullptr),
    text(std::make_shared<text_renderer>()) {
}

renderer::~renderer() {
    if (gl_context != nullptr) {
        SDL_GL_DeleteContext(gl_context);
    }
}

void renderer::bind(const window* w) {
    window_ = w;

    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    if (gl_context != nullptr) {
        SDL_GL_DeleteContext(gl_context);
    }

    gl_context = SDL_GL_CreateContext(w->get_handle());

    if (glewInit() != GLEW_OK) {
        LOG(ERROR) << "Could not initialize glew";
        throw;
    }

    text->load();

    SDL_GL_MakeCurrent(NULL, NULL);
}

void renderer::start_rendering() {
    SDL_GL_MakeCurrent(window_->get_handle(), gl_context);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    const auto size { window_->get_size() };
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, static_cast<GLsizei> (size.width), static_cast<GLsizei> (size.height));
    glOrtho(0.0f, static_cast<GLdouble> (size.width), static_cast<GLdouble> (size.height), 0.0f, 0.0f, 1.0f);
    glScalef(1.0f, 1.0f, 1.0f);
}

void renderer::finish_rendering() {
    SDL_GL_SwapWindow(window_->get_handle());
    SDL_GL_MakeCurrent(NULL, NULL);
}
