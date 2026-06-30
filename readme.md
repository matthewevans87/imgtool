# imgtool

A command-line image processing tool that applies a configurable pipeline of transforms to an image.

**Doge**

![Doge](./images/image.png)


**Transformed Doge**
(grayscale -> blur -> sobel -> threshold)

![Transformed Doge](./images/threshold.png)



## Build

```sh
cmake -B build && cmake --build build
```

## Usage

```
imgtool <input> <output> [transform...]
```

Transforms are applied left to right. Multiple transforms can be chained.

### Transforms

| Transform                                                            | Syntax                    | Description                            |
| -------------------------------------------------------------------- | ------------------------- | -------------------------------------- |
| Grayscale                                                            | `grayscale`               | Convert to grayscale                   |
| [Gaussian blur](https://en.wikipedia.org/wiki/Gaussian_blur)         | `blur` or `blur:<radius>` | Blur with optional radius (default: 2) |
| [Sobel edge detection](https://en.wikipedia.org/wiki/Sobel_operator) | `sobel`                   | Detect edges                           |
| Threshold                                                            | `threshold:<value>`       | Binarize at given intensity (0–255)    |

### Examples

```sh
# Convert to grayscale
imgtool input.png output.png grayscale

# Blur then detect edges
imgtool input.png edges.png blur:3 sobel

# Full edge-detection pipeline
imgtool input.png result.png grayscale blur sobel threshold:128
```

