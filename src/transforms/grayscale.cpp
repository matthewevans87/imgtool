#include "transforms/grayscale.h"
#include <algorithm> // std::clamp

void GrayscaleTransform::apply(Image &image)
{
    if (image.channels() < 3)
        return; // already grayscale or single channel

    // create new single channel image
    int w = image.width();
    int h = image.height();
    int c = image.channels();
    Image result(w, h, 1);

    for (int y = 0; y < h; ++y)
    {
        const uint8_t *src = image.row(y); // pointer to start of row
        uint8_t *dst = result.row(y);

        for (int x = 0; x < w; ++x)
        {
            // ITU-R BT.601 luminance weights
            float lum = 0.299f * src[x * c + 0]    // R
                        + 0.587f * src[x * c + 1]  // G
                        + 0.114f * src[x * c + 2]; // B
            dst[x] = static_cast<uint8_t>(std::clamp(lum, 0.0f, 255.0f));
        }
    }

    image = std::move(result); // move assignment -- no copy
}
