#include <iostream>
#include <string>
#include <memory>
#include "image.h"
#include "pipeline.h"
#include "transforms/grayscale.h"
#include "transforms/gaussian_blur.h"
#include "transforms/sobel.h"
#include "transforms/threshold.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: imgtool <input> <output> <transform...>\n"
                  << "Transforms: grayscale, blur[:radius], sobel, threshold<value>\n";
        return 1;
    }

    std::string input_path = argv[1];
    std::string output_path = argv[2];

    TransformPipeline pipeline;

    // Parse transform names from remaining args
    for (int i = 3; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "grayscale")
        {
            pipeline.add(std::make_unique<GrayscaleTransform>());
        }
        else if (arg.starts_with("blur"))
        {
            int radius = 2; // default
            if (arg.size() > 5)
                radius = std::stoi(arg.substr(5)); // "e.g., blur:3"
            pipeline.add(std::make_unique<GaussianBlurTransform>(radius));
        }
        else if (arg == "sobel")
        {
            pipeline.add(std::make_unique<SobelTransform>());
        }
        else if (arg.starts_with("threshold:"))
        {
            uint8_t value = static_cast<uint8_t>(std::stoi(arg.substr(10)));
            pipeline.add(std::make_unique<ThresholdTransform>(value));
        }
        else
        {
            std::cerr << "Unknown transform: " << arg << "\n";
            return 1;
        }
    }

    try
    {
        Image image(input_path);
        std::cout << "Processing: " << input_path << " ("
                  << image.width() << "x" << image.height() << ")\n";
        pipeline.run(image);
        image.save(output_path);
        std::cout << "Saved: " << output_path << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}