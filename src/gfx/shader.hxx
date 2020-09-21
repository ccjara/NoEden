#ifndef JARALYN_SHADER_HXX
#define JARALYN_SHADER_HXX

enum class j_shader_type : GLenum {
    vertex = GL_VERTEX_SHADER,
    geometry = GL_GEOMETRY_SHADER,
    fragment = GL_FRAGMENT_SHADER
};

/**
 * @brief Wrapper around a GL shader.
 *
 * Currently barely enough to provide an extension point
 * for the text_shader. There probably won't be any other shaders
 * in this game.
 */
class j_shader {
private:
    std::unordered_map<j_shader_type, GLuint> stages_;

    void clear_stages();
protected:

    GLuint program_ { 0 };

    /**
     * @brief Will be called before the shader program is used
     *
     * When a consumer of the shader use()s the shader, the prepare
     * method can be used to set shader uniforms, bind textures
     * to fragment shaders, etc.
     *
     * @see use()
     */
    virtual void prepare();

    /**
     * @brief Compiles and attaches a shader source of the given type.
     *
     * @returns true if compilation was successful.
     */
    bool compile(j_shader_type type, std::string_view source);

    /**
     * @brief Links all shader stages into a shader program.
     *
     * Linker errors cause this shader to be a null-program
     *
     * @returns true if linkage was successful.
     */
    bool link();
public:
    /**
     * @brief Creates the shader program resource
     */
    j_shader();

    /**
     * @brief Unloads the program and leftover shader stages
     */
    virtual ~j_shader();

    /**
     * @brief Unloads the shader program and resets its handle
     *
     * If you call use() after unloading the shader, the null-program will
     * be used instead.
     */
    void unload();

    /**
     * @brief Returns whether a shader is currently loaded
     */
    bool is_loaded() const;

    /**
     * @brief Uses the currently loaded shader program during rendering
     */
    void use();

    /**
     * @brief Returns the id of the currently loaded shader
     *
     * Returns 0 if none is loaded
     */
    GLint id() const;

    j_shader(const j_shader&) = delete;
    j_shader(j_shader&&) = delete;
    j_shader& operator=(j_shader&&) = delete;
    const j_shader& operator=(const j_shader&) = delete;
};

#endif
