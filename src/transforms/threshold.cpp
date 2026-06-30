#include "transforms/threshold.h"
#include "image.h"

ThresholdTransform::ThresholdTransform(int threshold)
    : threshold_(threshold)
{
}

void ThresholdTransform::apply(Image &image)
{
    int w = image.width();
    int h = image.height();
    Image result(w, h, 1);

    for (int x = 0; x < w; ++x)
    {
        for (int y = 0; y < h; ++y)
        {
            uint8_t val = image.at(x, y, 0);
            if (val < threshold_)
            {
                val = 0;
            }
            result.at(x, y, 0) = val;
        }
    }

    image = std::move(result);
}