#include "transforms/sobel.h"
#include <cmath>
#include <algorithm>

void SobelTransform::apply(Image &image)
{
    if (image.channels() != 1)
        return; // require grayscale input

    int w = image.width();
    int h = image.height();

    Image result(w, h, 1);

    // Sobel kernels:
    // Gx = [[-1, 0, 1], [-2, 0, 2], [-1, 0, 1]]  (horizontal gradient)
    // Gy = [[-1,-2,-1], [ 0, 0, 0], [ 1, 2, 1]]  (vertical gradient)
    // Skip border pixels (1-pixel margin) for simplicity

    for (int y = 1; y < h - 1; ++y)
    {
        for (int x = 1; x < w - 1; ++x)
        {
            // clang-format off
            float gx = -1.0f * image.at(x-1, y-1, 0)
                       +1.0f * image.at(x+1, y-1, 0)
                       -2.0f * image.at(x-1, y,   0)
                       +2.0f * image.at(x+1, y,   0)
                       -1.0f * image.at(x-1, y+1, 0)
                       +1.0f * image.at(x+1, y+1, 0);

            float gy = -1.0f * image.at(x-1, y-1, 0)
                       -2.0f * image.at(x,   y-1, 0)
                       -1.0f * image.at(x+1, y-1, 0)
                       +1.0f * image.at(x-1, y+1, 0)
                       +2.0f * image.at(x,   y+1, 0)
                       +1.0f * image.at(x+1, y+1, 0);
            // clang-format on

            // Gradient magnitude -- std::hypot is cleaner than sqrt(gx^2+gy^2)
            float mag = std::hypot(gx, gy);
            result.at(x, y, 0) = static_cast<uint8_t>(std::clamp(mag, 0.0f, 255.0f));
        }
    }
    image = std::move(result);
}