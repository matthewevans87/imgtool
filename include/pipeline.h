#pragma once
#include "transforms/image_transform.h"
#include <vector>
#include <memory> // unique_ptr
#include <string>

class TransformPipeline
{
public:
    // Takes ownership of the transform
    void add(std::unique_ptr<ImageTransform> transform);
    void run(Image &image) const;
    size_t size() const { return transforms_.size(); }

private:
    std::vector<std::unique_ptr<ImageTransform>> transforms_;
};