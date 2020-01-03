#include "frame_buffer.hxx"

frame_buffer::frame_buffer() {
    glGenFramebuffers(1, &name);
    glGenTextures(1, &color_texture_name);

    glBindTexture(GL_TEXTURE_2D, color_texture_name);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(
        GL_TEXTURE_2D, 0, GL_RGB, 512, 512, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr
    );

    bind();

    glFramebufferTexture2D(
        GL_DRAW_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        color_texture_name,
        0
    );

    if (!glCheckFramebufferStatus(GL_FRAMEBUFFER)) {
        LOG(ERROR) << "FB " << name << " not complete: "
                << std::hex << glCheckFramebufferStatus(GL_FRAMEBUFFER);
        throw;
    }

    unbind();
    glBindTexture(GL_TEXTURE_2D, 0);
}

frame_buffer::~frame_buffer() {
    glDeleteFramebuffers(1, &name);
    glDeleteTextures(1, &color_texture_name);
}

void frame_buffer::bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, name);
}

void frame_buffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint frame_buffer::get_texture() const noexcept {
    return color_texture_name;
}
