#include "image.h"

// stb_image: define implementation in exactly ONE .cpp file
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <cstring> // memcpy
#include <stdexcept>

Image::Image(const std::string &path)
{
    // stbi_load returns a malloc'd buffer; copy it into a new[]-owned buffer
    // so all Image instances use the same allocator and delete[] is always safe.
    uint8_t *raw = stbi_load(path.c_str(), &width_, &height_, &channels_, 0);
    if (!raw)
        throw std::runtime_error("Failed to load image: " + path);
    size_t n = static_cast<size_t>(width_) * height_ * channels_;
    data_ = new uint8_t[n];
    std::memcpy(data_, raw, n);
    stbi_image_free(raw);
}

Image::Image(int width, int height, int channels)
    : width_(width), height_(height), channels_(channels)
{
    data_ = new uint8_t[width_ * height_ * channels_](); // zero-initializers
}

Image::Image(const Image &other)
    : width_(other.width_), height_(other.height_), channels_(other.channels_)
{
    size_t n = width_ * height_ * channels_;
    data_ = new uint8_t[n];
    std::memcpy(data_, other.data_, n);
}

Image &Image::operator=(const Image &other)
{
    if (this == &other)
        return *this; // self-assignment guard
    delete[] data_;
    width_ = other.width_;
    height_ = other.height_;
    channels_ = other.channels_;
    size_t n = width_ * height_ * channels_;
    data_ = new uint8_t[n];
    std::memcpy(data_, other.data_, n);
    return *this;
}

Image::Image(Image &&other) noexcept
    : width_(other.width_), height_(other.height_), channels_(other.channels_), data_(other.data_)
{
    // leave other in valid but empty state -- critical for move semantics
    other.data_ = nullptr;
    other.width_ = 0;
    other.height_ = 0;
    other.channels_ = 0;
}

Image &Image::operator=(Image &&other) noexcept
{
    if (this == &other)
        return *this;
    delete[] data_;
    data_ = other.data_;
    width_ = other.width_;
    height_ = other.height_;
    channels_ = other.channels_;
    other.data_ = nullptr;
    return *this;
}

Image::~Image()
{
    delete[] data_; // safe to delete nullptr
}

void Image::save(const std::string &path) const
{
    // stb_image_write infers format from extension
    int ok = stbi_write_png(path.c_str(), width_, height_, channels_, data_, width_ * channels_); // stride in bytes
    if (!ok)
        throw std::runtime_error("Failed to save image: " + path);
}
