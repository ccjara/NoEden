#ifndef JARALYN_TEXT_SHADER_HXX
#define JARALYN_TEXT_SHADER_HXX

#include "shader.hxx"
#include "texture.hxx"

/**
 * @brief Shader which renders the entire game screen.
 */
class j_text_shader : public j_shader {
private:
    GLint u_resolution_; // render resolution (view port divided by scaling)
    GLint u_glyph_size_; // size of an individual character
    GLint u_tex_size_; // size of the bitmap font texture

    j_texture tex_;
    j_size<uint32_t> glyph_size_;
    j_size<uint32_t> resolution_;

    void prepare() override;
public:
    j_text_shader();

    /**
     * @brief Configures the shader to use the given size for each glyph.
     */
    void use_glyph_size(j_size<uint32_t> glyph_size);

    /**
     * @brief Uses the given, loaded texture as part of the shader program
     */
    void use_texture(j_texture&& tex);

    /**
     * @brief Configures the render resolution of the shader program
     */
    void use_resolution(j_size<uint32_t> resolution);
};

#endif
