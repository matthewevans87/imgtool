#pragma once
#include <cstdint> //uint8_t
#include <string>
#include <stdexcept>

class Image
{
public:
    // constructor: load from file
    explicit Image(const std::string &path);

    // constructor: create blank image (width x height, n channels, all zeros)
    Image(int width, int height, int channels);

    // Rule of Five: because we manage raw memory, we must define all five.
    // Copy constructor -- deep copy the pixel buffer
    Image(const Image &other);
    // Copy assignment
    Image &operator=(const Image &other);
    // Move constructor -- steal the buffer, leave other in valid empty state
    Image(Image &&other) noexcept;
    // Move assignment
    Image &operator=(Image &&other) noexcept;
    // Destructor -- free the buffer
    ~Image();

    // Accessors
    int width() const { return width_; }
    int height() const { return height_; }
    int channels() const { return channels_; }

    // Raw pixel access -- this is where pointer arithmetic lives.
    // Pixel at (x, y), channel c:
    //  data_[ (y * width_ + x) * channels + c ]
    uint8_t *data() { return data_; }
    const uint8_t *data() const { return data_; }

    uint8_t *row(int y)
    {
        return data_ + y * width_ * channels_;
    }

    const uint8_t *row(int y) const
    {
        return data_ + y * width_ * channels_;
    }

    // Get/set a single pixel value
    uint8_t &at(int x, int y, int c)
    {
        return data_[(y * width_ + x) * channels_ + c];
    }
    uint8_t &at(int x, int y, int c) const
    {
        return data_[(y * width_ + x) * channels_ + c];
    }

    void save(const std::string &path) const;

private:
    int width_ = 0;
    int height_ = 0;
    int channels_ = 0;
    uint8_t *data_ = nullptr; // heap-allocated pixel buffer
};