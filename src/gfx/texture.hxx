#ifndef JARALYN_TEXTURE_HXX
#define JARALYN_TEXTURE_HXX

#include "surface.hxx"

class j_texture {
private:
    GLuint id_;

    j_size<uint32_t> size_;
public:
    GLuint id() const noexcept;

    ~j_texture();
    
    void load(const std::string& path);
    void unload();
    bool is_loaded() const noexcept;
    j_size<uint32_t> size() const noexcept;

    void bind() noexcept;
    void unbind() noexcept;
};

#endif
