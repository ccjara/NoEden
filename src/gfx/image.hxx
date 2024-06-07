#ifndef NOEDEN_IMAGE_HXX
#define NOEDEN_IMAGE_HXX

/**
 * @brief Janitor wrapper around stb_image
 */
class Image {
public:
    /**
     * @brief Loads an image from the given path
     *
     * @param path Path to the image file
     * @return Loaded image
     */
    [[nodiscard]] static Image from_file(std::string_view path);

    /**
     * @brief Provides access to the raw image data.
     */
    [[nodiscard]] unsigned char* data() const;

    /**
     * @brief Width of the image in pixels
     */
    [[nodiscard]] i32 width() const;

    /**
     * @brief Height of the image in pixels
     */
    [[nodiscard]] i32 height() const;

    /**
     * @brief Number of channels in the loaded image
     */
    [[nodiscard]] i32 channels() const;

    ~Image();
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