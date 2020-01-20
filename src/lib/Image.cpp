/* IMAGE.cpp
 *   by Lut99
 *
 * Created:
 *   1/20/2020, 3:03:07 PM
 * Last edited:
 *   1/20/2020, 3:35:17 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Image class is meant to be raw image storage. It is not much more
 *   than a wrapped 3D-array (that is allocated in 1D) with the added bonus
 *   of easy to-file features. This particular file is the decleration file
 *   for Image.hpp.
**/

#include <fstream>
#include <stdexcept>
#include <iostream>
#include <cerrno>
#include <cstring>

#include "include/Image.hpp"

using namespace std;
using namespace RayTracer;


Image::Image(std::size_t width, std::size_t height)
    : width(width),
    height(height)
{
    this->data = new double[this->width * this->height * 3];
}
Image::~Image() {
    delete[] this->data;
}

double& Image::pixel(std::size_t row, std::size_t col, std::size_t channel) {
    // Check if the indices are within bounds
    if (row >= this->width || col >= this->height || channel >= 3) {
        throw out_of_range("Pixel at (" + to_string(col) + ", " + to_string(row) + ", " + to_string(channel) + ") is out of range for Image with size (" + to_string(this->width) + ", " + to_string(this->height) + ", 3)");
    }

    // Get the correct pixel
    return this->data[row * this->height + col * this->width + channel];
}

void Image::to_ppm(std::string path) {
    // Get a file handle
    ofstream out(path);
    if (!out.is_open()) {
        cerr << "Could not open file: " << strerror(errno) << endl;
        exit(1);
    }

    // Write the header
    out << "P3" << endl;
    // Write the size
    out << this->width << ' ' << this->height << endl;
    // Write colour max(?)
    out << 255 << endl;
    // Write all pixels, row-by-row
    for (std::size_t y = 0; y < this->height; y++) {
        for (std::size_t x = 0; x < this->height; x++) {
            if (x > 0) {
                out << ' ';
            }
            // Get the three colours as 0-255
            char red = 255 * this->pixel(y, x, 0);
            char green = 255 * this->pixel(y, x, 1);
            char blue = 255 * this->pixel(y, x, 2);

            out << red << ' ' << green << ' ' << blue;
        }
        out << endl;
    }

    // Close the file and done
    out.close();
}