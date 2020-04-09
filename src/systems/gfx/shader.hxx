#ifndef JARALYN_SHADER_HXX
#define JARALYN_SHADER_HXX

class shader {
protected:
    GLuint program_ { 0 };
public:
    virtual ~shader();

    /**
     * @brief Loads a simple vertex + fragment shader from the given source
     *
     * The vertex + fragment shader program shader is compiled and linked
     * from the given sources. If a shader program was previously loaded
     * the previous program will be unloaded beforehand.
     */
    void load_source(const std::string& vertex_source, const std::string& framgent_source);

    /**
     * @brief Unloads all existing resources and resets the program handle
     *
     * If you call use() after unloading the shader, the null-program will
     * be used instead.
     */
    void unload();

    /**
     * @brief Returns whether a shader is currently loaded
     */
    bool is_loaded() const noexcept;

    /**
     * @brief Uses the currently loaded shader program during rendering
     */
    void use() const noexcept;

    /**
     * @brief Returns the id of the currently loaded shader
     *
     * Returns 0 if none is loaded
     */
    GLint id() const noexcept;
};

#endif
