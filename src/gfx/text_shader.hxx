#ifndef NOEDEN_TEXT_SHADER_HXX
#define NOEDEN_TEXT_SHADER_HXX

#include "shader.hxx"
#include "texture.hxx"

/**
 * @brief Shader which renders the entire game screen.
 */
class TextShader : public Shader {
public:
    TextShader();

    /**
     * @brief Configures the shader to use the given size for each glyph.
     */
    void use_glyph_size(Vec2<u32> glyph_size);

    /**
     * @brief Uses the given, loaded texture as part of the shader program
     */
    void use_texture(Texture *tex);

    /**
     * @brief Configures the render resolution of the shader program
     */
    void use_resolution(Vec2<u32> resolution);

    /**
     * @brief Readonly accessor of the glyph size in pixels
     */
    Vec2<u32> glyph_size() const;

    /**
     * @brief Readonly accessor of the texture size in pixels
     */
    Vec2<u32> texture_size() const;

private:
    GLint u_resolution_; // render resolution (view port divided by scaling)
    GLint u_glyph_size_; // size of an individual character
    GLint u_tex_size_; // size of the bitmap font texture

    Texture *tex_ = nullptr;
    Vec2<u32> glyph_size_;
    Vec2<u32> resolution_;

    void prepare() override;
};

#endif
