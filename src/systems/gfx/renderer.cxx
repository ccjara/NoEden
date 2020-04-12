#include "renderer.hxx"

renderer::renderer() :
    gl_context(nullptr) {
}

renderer::~renderer() {
    if (gl_context != nullptr) {
        glDeleteBuffers(1, &vao);
        glDeleteBuffers(1, &vbo);

        SDL_GL_DeleteContext(gl_context);
    }
}

void renderer::bind(const window* w) {
    window_ = w;

    SDL_GL_SetSwapInterval(1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

    if (gl_context != nullptr) {
        SDL_GL_DeleteContext(gl_context);
    }

    gl_context = SDL_GL_CreateContext(w->get_handle());

    if (glewInit() != GLEW_OK) {
        LOG(ERROR) << "Could not initialize glew";
        throw;
    }

    tex_.load("font.bmp");

    /////////////////////////////////////////////////////////////////

    text_shader_.load(&tex_);
    text_shader_.use_view_port(w->get_size());
    text_shader_.use_glyph_size({ 8, 14 });

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 500 * sizeof(letter), nullptr, GL_DYNAMIC_DRAW);

    // vertices
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(letter), nullptr);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(letter), reinterpret_cast<void*> (sizeof(letter::sign)));
    glEnableVertexAttribArray(1);

    /////////////////////////////////////////////////////////////////

    SDL_GL_MakeCurrent(nullptr, nullptr);
}

void renderer::start_rendering() {
    SDL_GL_MakeCurrent(window_->get_handle(), gl_context);

    const auto size { window_->get_size() };

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, size.width, size.height);

    letter next_data[500];

    rot++;
    if (rot + 500 > 754) {
        rot = 0;
    }

    for (int i = 0; i < 500; i++) {
        next_data[i] = {
           (float) i + rot, 1.0f, 1.0f, 1.0f,
        };
    }

    text_shader_.use();

    glBindVertexArray(vao);

    void *data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    memcpy(data, next_data, sizeof(next_data));

    glUnmapBuffer(GL_ARRAY_BUFFER);
    
    glDrawArrays(GL_POINTS, 0, 500);
}

void renderer::finish_rendering() {
    SDL_GL_SwapWindow(window_->get_handle());
    SDL_GL_MakeCurrent(nullptr, nullptr);
}
