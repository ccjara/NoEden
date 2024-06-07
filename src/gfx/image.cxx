#include "image.hxx"

Image Image::from_file(std::string_view path) {
    Image img;
    img.data_ = stbi_load(path.data(), &img.width_, &img.height_, &img.channels_, 0);
    return img;
}

Image::~Image() {
    if (data_) {
        stbi_image_free(data_);
        data_ = nullptr;
    }
}

unsigned char* Image::data() const {
    return data_;
}

i32 Image::width() const {
    return width_;
}

i32 Image::height() const {
    return height_;
}

i32 Image::channels() const {
    return channels_;
}
