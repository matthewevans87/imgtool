#include "pipeline.h"
#include <iostream>

void TransformPipeline::add(std::unique_ptr<ImageTransform> transform)
{
    transforms_.push_back(std::move(transform));
}

void TransformPipeline::run(Image &image) const
{
    for (const auto &transform : transforms_)
    {
        std::cout << "Applying: " << transform->name() << "\n";
        transform->apply(image);
    }
}