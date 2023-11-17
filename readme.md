# C Mandelbrot

## Overview

The C_Mandelbrot project is a C program for generating Mandelbrot fractal images. The program provides flexibility in rendering Mandelbrot sets with various color schemes and visualization options. It allows users to customize the rendering parameters such as maximum iterations, camera position, image dimensions, and output file basename.

## Table of Contents

- [C Mandelbrot](#c-mandelbrot)
  - [Overview](#overview)
  - [Table of Contents](#table-of-contents)
  - [Compilation](#compilation)
  - [Usage](#usage)
  - [Configuration](#configuration)
  - [Output Types](#output-types)
  - [Examples](#examples)
    - [Example 1](#example-1)
    - [Example 2](#example-2)
  - [License](#license)

## Compilation

To compile the program, use the following commands:

```bash
gcc -W -Wall -ansi -pedantic -c colors.c
gcc -W -Wall -ansi -pedantic -c mandel.c
gcc -W -Wall -ansi -pedantic colors.o mandel.o -o mandel -lm
```

## Usage

The program takes command-line arguments for customization. The basic usage is as follows:

```bash
./mandel [max_iter] [cam_x,cam_y,cam_w,cam_h] [img_widthximg_height] [basename]
```

- `max_iter`: Maximum number of iterations for the Mandelbrot set calculations.
- `cam_x,cam_y,cam_w,cam_h`: Camera position and dimensions (coordinates and width/height).
- `img_widthximg_height`: Image dimensions in width x height format.
- `basename`: Basename for the output files.

## Configuration

Alternatively, users can provide a configuration file with the desired parameters. An example configuration file might look like this:

```plaintext
output_image
1000x1000
rgb
-0.76,0,2.48,2.48
100
2
```

To use a configuration file, pass its path as the first command-line argument:

```bash
./mandel config.txt
```

## Output Types

The program supports various output types, which can be specified in the configuration or command-line arguments:

- `rgb`: Color image (default)
- `b&w`: Black and white image
- `b&w_alt`: Alternating black and white
- `grey_stepped`: Grey levels with stepped interpolation
- `grey_smoothed`: Smoothed grey levels

## Examples

### Example 1

Generate a default Mandelbrot image with 1000 iterations, a default camera position, and dimensions of 1000x1000 pixels. Save the output with the basename \"mandel\".

```bash
./mandel 1000 -0.76,0,2.48,2.48 1000x1000 output_image
```

### Example 2

Generate a Mandelbrot image using a configuration file named \"config.txt\".

```bash
./mandel config.txt
```

## License

This project is licensed under the [MIT License](LICENSE). Feel free to use and modify according to your needs.
