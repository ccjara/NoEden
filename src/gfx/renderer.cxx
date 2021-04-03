#include "renderer.hxx"

j_renderer::~j_renderer() {
    reset();
}

void j_renderer::render(const j_display& display) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, view_port_.x, view_port_.y);

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
    glDrawArrays(GL_POINTS, 0, display.cell_count());
}

void j_renderer::set_viewport(j_vec2<uint32_t> size) {
    view_port_ = size;
    text_shader_->use_resolution(view_port_ / scaling_);
}

void j_renderer::set_font(j_texture* tex) {
    text_shader_->use_texture(tex);
}

void j_renderer::set_glyph_size(j_vec2<uint32_t> glyph_size) {
    text_shader_->use_glyph_size(glyph_size);
}

void j_renderer::set_scaling(uint32_t scaling) {
    scaling_ = scaling;
    text_shader_->use_resolution(view_port_ / scaling);
}

void j_renderer::reset() {
    gl_context_ = nullptr; // not owned by this class
    if (vao) {
        glDeleteBuffers(1, &vao);
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }
}

void j_renderer::set_context(SDL_GLContext context) {
    reset();

    gl_context_ = context;

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // glyph
    glVertexAttribIPointer(0, 1, GL_INT, sizeof(j_display::cell_type), nullptr);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(j_display::cell_type), reinterpret_cast<void*> (sizeof(j_display::cell_type::glyph)));
    glEnableVertexAttribArray(1);

    text_shader_ = std::make_unique<j_text_shader>();
}
