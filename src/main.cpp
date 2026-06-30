#include <iostream>
#include <string>
#include <memory>
#include <filesystem>
#include "image.h"
#include "pipeline.h"
#include "transforms/grayscale.h"
#include "transforms/gaussian_blur.h"
#include "transforms/sobel.h"
#include "transforms/threshold.h"
#include <future>
#include <vector>

namespace fs = std::filesystem;

bool process_file(const std::string &input, const std::string &output, const TransformPipeline &pipeline)
{
    try
    {
        Image image(input);
        pipeline.run(image);
        image.save(output);
        return true;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error processing " << input << ": " << e.what() << "\n";
        return false;
    }
}

static bool is_transform(const std::string &s)
{
    return s == "grayscale" || s == "sobel" ||
           s.starts_with("blur") || s.starts_with("threshold:");
}

static void add_transform(TransformPipeline &pipeline, const std::string &arg)
{
    if (arg == "grayscale")
        pipeline.add(std::make_unique<GrayscaleTransform>());
    else if (arg.starts_with("blur"))
    {
        int radius = 2;
        if (arg.size() > 5)
            radius = std::stoi(arg.substr(5));
        pipeline.add(std::make_unique<GaussianBlurTransform>(radius));
    }
    else if (arg == "sobel")
        pipeline.add(std::make_unique<SobelTransform>());
    else if (arg.starts_with("threshold:"))
    {
        uint8_t value = static_cast<uint8_t>(std::stoi(arg.substr(10)));
        pipeline.add(std::make_unique<ThresholdTransform>(value));
    }
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: imgtool <input> <output> [transform...]\n"
                  << "       imgtool --batch <input_dir> <output_dir> [transform...]\n"
                  << "Transforms: grayscale, blur[:radius], sobel, threshold:<value>\n";
        return 1;
    }

    bool isBatch = std::string(argv[1]) == "--batch";
    int inputArg = isBatch ? 2 : 1;
    int outputArg = isBatch ? 3 : 2;
    int transformsStart = isBatch ? 4 : 3;

    if (argc <= outputArg)
    {
        std::cerr << "Error: missing " << (isBatch ? "output directory" : "output path") << ".\n";
        return 1;
    }

    TransformPipeline pipeline;
    for (int i = transformsStart; i < argc; ++i)
    {
        std::string arg = argv[i];
        if (!is_transform(arg))
        {
            std::cerr << "Unknown transform: " << arg << "\n";
            return 1;
        }
        add_transform(pipeline, arg);
    }

    if (!isBatch)
    {
        try
        {
            Image image(argv[inputArg]);
            std::cout << "Processing: " << argv[inputArg] << " ("
                      << image.width() << "x" << image.height() << ")\n";
            pipeline.run(image);
            image.save(argv[outputArg]);
            std::cout << "Saved: " << argv[outputArg] << "\n";
        }
        catch (const std::exception &e)
        {
            std::cerr << "Error: " << e.what() << "\n";
            return 1;
        }
        return 0;
    }

    // Batch mode: process all .png files in input_dir, write to output_dir
    fs::path inputDir = argv[inputArg];
    fs::path outputDir = argv[outputArg];

    if (!fs::is_directory(inputDir))
    {
        std::cerr << "Error: input path is not a directory: " << inputDir << "\n";
        return 1;
    }

    std::error_code ec;
    fs::create_directories(outputDir, ec);
    if (ec)
    {
        std::cerr << "Error: could not create output directory: " << ec.message() << "\n";
        return 1;
    }

    std::vector<std::future<bool>> futures;
    for (const auto &entry : fs::directory_iterator(inputDir))
    {
        if (!entry.is_regular_file() || entry.path().extension() != ".png")
            continue;

        fs::path outPath = outputDir / entry.path().filename();
        futures.push_back(std::async(std::launch::async,
                                     process_file, entry.path().string(), outPath.string(), std::cref(pipeline)));
    }

    if (futures.empty())
    {
        std::cerr << "No .png files found in " << inputDir << "\n";
        return 1;
    }

    bool allOk = true;
    for (auto &f : futures)
        allOk &= f.get();

    return allOk ? 0 : 1;
}