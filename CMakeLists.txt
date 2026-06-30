cmake_minimum_required(VERSION 3.20)
project(imgtool CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Wextra -Wpedantic)

add_executable(imgtool
    src/main.cpp
    src/image.cpp
    src/transforms/grayscale.cpp
    src/transforms/gaussian_blur.cpp
    src/transforms/sobel.cpp
    src/transforms/threshold.cpp
    src/pipeline.cpp
)

target_include_directories(imgtool PRIVATE include third_party)