#ifndef JARALYN_TEXT_SHADER_HXX
#define JARALYN_TEXT_SHADER_HXX

#include "shader.hxx"
#include "texture.hxx"

class j_text_shader : public j_shader {
private:
    GLint u_view_port_; // size of the view port (j_window inner size)
    GLint u_glyph_size_; // size of an individual character
    GLint u_tex_size_; // size of the bitmap font texture

    j_texture* tex_ { nullptr };
    j_size<float_t> glyph_size_ { 0.0f, 0.0f };
    j_size<float_t> view_port_ { 0.0f, 0.0f };

    void prepare() override;
public:
    void load(j_texture* tex);

    void use_glyph_size(j_size<uint32_t> glyph_size);
    void use_texture(j_texture* tex);
    void use_view_port(j_size<uint32_t> view_port);
};

#endif
