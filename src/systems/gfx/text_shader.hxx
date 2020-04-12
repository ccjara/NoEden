#ifndef JARALYN_TEXT_SHADER_HXX
#define JARALYN_TEXT_SHADER_HXX

#include "shader.hxx"
#include "texture.hxx"

class text_shader : public shader {
private:
    GLint u_view_port_; // size of the view port (window inner size)
    GLint u_glyph_size_; // size of an individual character
    GLint u_tex_size_; // size of the bitmap font texture

    texture* tex_ { nullptr };
    size<float_t> glyph_size_ { 0.0f, 0.0f };
    size<float_t> view_port_ { 0.0f, 0.0f };

    void prepare() override;
public:
    void load(texture* tex);

    void use_glyph_size(size<uint32_t> glyph_size);
    void use_texture(texture* tex);
    void use_view_port(size<uint32_t> view_port);
};

#endif
