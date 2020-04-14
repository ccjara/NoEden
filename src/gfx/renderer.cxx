#include "renderer.hxx"

j_renderer::~j_renderer() {
    if (vao) {
        glDeleteBuffers(1, &vao);
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }
}

void j_renderer::bind(const j_window* w) {
    assert(w);
    window_ = w;

    tex_.load("font.bmp");

    text_shader_ = std::make_unique<j_text_shader>(&tex_);
    text_shader_->use_view_port(w->size());
    text_shader_->use_glyph_size({ 8, 14 });

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // glyph
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(j_display::cell_type), nullptr);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(j_display::cell_type), reinterpret_cast<void*> (sizeof(j_display::cell_type::glyph)));
    glEnableVertexAttribArray(1);
}

void j_renderer::render(const j_display& display) {
    const auto size { window_->size() };

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, size.width, size.height);

    text_shader_->use();

    glBindVertexArray(vao);

    const auto byte_size { display.byte_size() };

    if (last_size_ == byte_size) {
        auto data = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

        std::memcpy(data, display.data(), byte_size);

        glUnmapBuffer(GL_ARRAY_BUFFER);
    } else {
        // glBindBuffer(GL_ARRAY_BUFFER, vbo); // not necessary - autobound by vao
        glBufferData(GL_ARRAY_BUFFER, byte_size, display.data(), GL_DYNAMIC_DRAW);

        last_size_ = byte_size;
    }
    glDrawArrays(GL_POINTS, 0, display.size());

    SDL_GL_SwapWindow(window_->handle());
}
