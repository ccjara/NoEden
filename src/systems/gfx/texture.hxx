#ifndef JARALYN_TEXTURE_HXX
#define JARALYN_TEXTURE_HXX

#include "surface.hxx"

class texture {
private:
    GLuint id_;

    size<uint32_t> size_;
public:
    GLuint id() const noexcept;

    ~texture();
    
    void load(const std::string& path);
    void unload();
    bool is_loaded() const noexcept;
    size<uint32_t> get_size() const noexcept;

    void bind() noexcept;
    void unbind() noexcept;
};

#endif
