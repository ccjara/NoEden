#ifndef NOEDEN_IMAGE_HXX
#define NOEDEN_IMAGE_HXX

/**
 * @brief Janitor wrapper around stb_image
 */
class Image {
public:
    [[nodiscard]] static Image from_file(std::string_view path);

    ~Image();

    [[nodiscard]] unsigned char* data() const;

    [[nodiscard]] i32 width() const;
    [[nodiscard]] i32 height() const;

    [[nodiscard]] i32 channels() const;

    Image(const Image&) = delete;
    Image(Image&&) = default;
    Image& operator=(const Image&) = delete;
    Image& operator=(Image&&) = default;
private:
    Image() = default;

    unsigned char* data_ = nullptr;
    i32 width_ = 0;
    i32 height_ = 0;
    i32 channels_ = 0;
};

#endif