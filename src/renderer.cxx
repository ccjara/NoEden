#include "renderer.hxx"

renderer::renderer() :
    gl_context(nullptr),
    text(std::make_shared<text_renderer>()) {
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

renderer::~renderer() {
    if (gl_context != nullptr) {
        SDL_GL_DeleteContext(gl_context);
    }
}

void renderer::bind(std::shared_ptr<window> w) {
    wnd = w;
    gl_context = SDL_GL_CreateContext(w->get_handle());
    SDL_GL_SetSwapInterval(1);

    text->load();
}

void renderer::start_rendering() {
    const auto size { wnd->get_size() };
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, static_cast<GLsizei> (size.x), static_cast<GLsizei> (size.y));
    glOrtho(0.0f, static_cast<GLdouble> (size.x), static_cast<GLdouble> (size.y), 0.0f, 0.0f, 1.0f);
    // glTranslatef(0.5f, 0.5f, 0.0f);
    glScalef(2.0f, 2.0f, 1.0f);
}

void renderer::finish_rendering() {
    SDL_GL_SwapWindow(wnd->get_handle());
}
