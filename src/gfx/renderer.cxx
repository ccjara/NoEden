#include "renderer.hxx"

j_renderer::j_renderer() {
    tex_.load("font.bmp");

    text_shader_ = std::make_unique<j_text_shader>(&tex_);
    text_shader_->use_glyph_size({ 8, 14 });

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // glyph
    glVertexAttribIPointer(0, 1, GL_INT, sizeof(j_display::cell_type), nullptr);
    glEnableVertexAttribArray(0);

    // color
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(j_display::cell_type), reinterpret_cast<void*> (sizeof(j_display::cell_type::glyph)));
    glEnableVertexAttribArray(1);
}

j_renderer::~j_renderer() noexcept {
    if (vao) {
        glDeleteBuffers(1, &vao);
    }
    if (vbo) {
        glDeleteBuffers(1, &vbo);
    }
}

void j_renderer::render(const j_display& display) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, view_port_.width, view_port_.height);

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
}

void j_renderer::set_viewport(j_size<uint32_t> size) noexcept {
    view_port_ = size;
    text_shader_->use_view_port(view_port_);
}
