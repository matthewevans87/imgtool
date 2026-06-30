#pragma once
#include "transforms/image_transform.h"

class ThresholdTransform : public ImageTransform
{
public:
    ThresholdTransform(int threshold);
    std::string name() const override { return "threshold"; }
    void apply(Image &image) override;
    int threshold() const { return threshold_; }

private:
    int threshold_ = 255;
};