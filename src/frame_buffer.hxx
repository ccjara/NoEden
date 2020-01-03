#ifndef JARALYN_FRAME_BUFFER_HXX
#define JARALYN_FRAME_BUFFER_HXX

#include "ide_pch.hxx"

class frame_buffer {
private:
    GLuint name { 0 };
    GLuint color_texture_name { 0 };
public:
    frame_buffer();
    frame_buffer(const frame_buffer&) = delete;
    ~frame_buffer();

    void bind();
    void unbind();

    [[nodiscard]] GLuint get_texture() const noexcept;
};


#endif
