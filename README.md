# PPM Image Denoiser

A C++ tool that denoises Portable Pixmap (PPM) images by averaging multiple noisy samples.

## Overview
This program takes a set of 10 noisy PPM images and produces a single clean image by averaging the pixel values across all input files. The idea is that random noise cancels out when you average enough samples of the same image.


## Tech Stack
* **Language:** C++
* **Build System:** CMake
* **Key Concepts:** File I/O, 2D Arrays/Vectors, Memory Management, Multi-stream processing

## Setup & How to Run

### Build Instructions
```bash
mkdir build && cd build
cmake ..
make
```

### Execution
The program looks for 10 files named `<base_name>_noisy_1.ppm` through `<base_name>_noisy_10.ppm`.

To run the program:
```bash
./proj1 ../images/image01
```
*(This passes `../images/image01` as the base name, processing `image01_noisy_1.ppm` through `10` and outputting `image01_clean.ppm`)*

## Input/Output Format
**Input:** 10 noisy PPM files.  
**Output:** A single cleaned image saved as `<base_name>_clean.ppm`.

## Functions
| Function | Description |
|---|---|
| `verify_input_files` | Checks that all 10 input files can be opened |
| `get_image_header` | Reads PPM version, width, height, and max pixel value |
| `open_output_file` | Creates the output clean PPM file |
| `write_image_header` | Writes the header to the output file |
| `open_input_streams` | Opens all 10 noisy files as active streams |
| `skip_stream_headers` | Advances each stream past its header data |
| `get_next_pixel_average` | Averages the current pixel value across all streams |
| `write_pixel_data` | Loops thru all pixels and writes averaged values |
| `generate_clean_image` | Coordinates the full denoising pipeline |

## Challenges & Solutions
* **The Challenges:** Managing 10 simultaneous file streams and ensuring that the headers were correctly skipped for each stream before performing pixel-by-pixel averaging. Handling file paths and validating the inputs was also crucial to prevent memory leaks or crashes.
* **The Solutions:** Created modular helper functions (`open_input_streams`, `skip_stream_headers`) to handle file operations systematically. Kept track of streams using a vector/array, moving the file pointers past the headers before iterating through the pixel data in a synchronized loop.

## Future Work
* **Dynamic Image Counts:** Allow the program to automatically detect the number of `_noisy_X` files or take it as a command-line argument instead of hardcoding 10 images.
* **Add Multithreading:** Since each pixel is calculated independently, multithreading (using `std::thread` or OpenMP) could significantly speed up processing for high-resolution images.
* **Median Filtering:** Use the median instead of the mean to better remove "salt and pepper" noise without blurring image edges.
* **CLI Flags/Arguments:** Support flags like `-i <input_base>`, `-o <output_name>`, and `-m <mean|median>`.
* **Support for PNG/JPG:** Integrate a library like `stb_image` to handle modern image formats rather than just PPMs.

---

> "Check out more of my work and my full portfolio at [luiscortesjimenez.github.io](https://luiscortesjimenez.github.io/)."
