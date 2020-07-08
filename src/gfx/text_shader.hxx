#ifndef JARALYN_TEXT_SHADER_HXX
#define JARALYN_TEXT_SHADER_HXX

#include "shader.hxx"
#include "texture.hxx"

/**
 * @brief Shader which renders the entire game screen.
 */
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
    explicit j_text_shader(j_texture* tex);

    /**
     * @brief Configures the shader to use the given size for each glyph.
     */
    void use_glyph_size(j_size<uint32_t> glyph_size);

    /**
     * @brief Passes the given texture into the shader program
     *
     * Does not claim ownership, thus requiring the texture to be
     * always available during render.
     */
    void use_texture(j_texture* tex);

    /**
     * @brief Passes the current view port into the shader program
     *
     * Should represent the currently configured GL view port, otherwise
     * the glyphs may look distorted.
     */
    void use_view_port(j_size<uint32_t> view_port);
};

#endif
