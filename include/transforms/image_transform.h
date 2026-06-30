#pragma once
#include "image.h"
#include <string>

class ImageTransform
{
public:
    // virtual ... = 0 --> "Pure virtual function" -think-> "abstract" method.
    // All functions are marked as pure virtual, hence an "abstract class"
    // All subclasses must implement all pure virtual functions, or they too will be abstract classes.
    virtual void apply(Image &image) = 0;
    virtual std::string name() const = 0;
    virtual ~ImageTransform() = default;
};