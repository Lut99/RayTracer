/* IMAGE.cpp
 *   by Lut99
 *
 * Created:
 *   1/20/2020, 3:03:07 PM
 * Last edited:
 *   1/22/2020, 1:54:50 PM
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
#include "include/lodepng.h"

using namespace std;
using namespace RayTracer;

/* PIXEL CLASS */
Pixel::Pixel(double* data) {
    // Store the values
    this->data = data;
}

double& Pixel::r() {
    return this->data[0];
}
double& Pixel::g() {
    return this->data[1];
}
double& Pixel::b() {
    return this->data[2];
}

double& Pixel::operator[](int index) {
    if (index < 0 || index >= 3) {
        throw out_of_range("Cannot get colour channel " + to_string(index) + " from Image with 3 channels");
    }

    return this->data[index];
}
Pixel& Pixel::operator=(const Vec3& other) {
    // Simply set our own data to given vector
    this->data[0] = other.x;
    this->data[1] = other.y;
    this->data[2] = other.z;
    return *this;
}


/* IMAGE ROW CLASS */
ImageRow::ImageRow(double* data, int width)
    : width(width)
{
    this->data = data;
}

Pixel ImageRow::operator[](int index) {
    // Check if out of bounds
    if (index < 0 || index >= this->width) {
        throw out_of_range("Cannot get element " + to_string(index) + " from Image with width " + to_string(index));
    }

    // Get a pointer to the correct position in the data
    double* pixel = this->data + 3 * index;
    return Pixel(pixel);
}


/* IMAGE CLASS */
Image::Image(int width, int height)
    : width(width),
    height(height)
{
    this->data = new double[this->width * this->height * 3];
}
Image::~Image() {
    delete[] this->data;
}

Pixel Image::pixel(int row, int col) {
    // Check if the indices are within bounds
    if (row < 0 || row >= this->width || col < 0 || col >= this->height) {
        throw out_of_range("Pixel at (" + to_string(col) + ", " + to_string(row) + ") is out of range for Image with size (" + to_string(this->width) + ", " + to_string(this->height) + ")");
    }

    // Get a pointer to the data
    double* pixel = this->data + 3 * (row * this->width + col);
    return Pixel(pixel);
}

ImageRow Image::operator[](int index) {
    // Check if the index is within bounds
    if (index < 0 || index >= this->height) {
        throw out_of_range("Cannot get row " + to_string(index) + " from Image with height " + to_string(index));
    }

    // Get the correct pointer
    double* row = this->data + 3 * (index * this->width);
    return ImageRow(row, this->width);
}

void Image::to_png(std::string path) {
    // Generate a 0-255, four channel vector
    std::vector<unsigned char> raw_image;
    raw_image.resize(this->width * this->height * 4);
    for (std::size_t y = 0; y < this->height; y++) {
        for (std::size_t x = 0; x < this->width; x++) {
            // Store the data as 0-255 Red Green Blue Alhpa
            raw_image[4 * (y * this->width + x) + 0] = 255 * this->operator[](y)[x][0];
            raw_image[4 * (y * this->width + x) + 1] = 255 * this->operator[](y)[x][1];
            raw_image[4 * (y * this->width + x) + 2] = 255 * this->operator[](y)[x][2];
            raw_image[4 * (y * this->width + x) + 3] = 255;
        }
    }

    // Write that to the output file using LodePNG
    unsigned result = lodepng::encode(path.c_str(), raw_image, this->width, this->height);
    if (result != 0) {
        cerr << "Could not write to PNG file: " << lodepng_error_text(result) << endl;
        exit(1);
    }
}