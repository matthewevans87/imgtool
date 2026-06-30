#pragma once
#include "transforms/image_transform.h"

class GrayscaleTransform : public ImageTransform
{
public:
    void apply(Image &image) override;
    std::string name() const
    {
        return "grayscale";
    }
};