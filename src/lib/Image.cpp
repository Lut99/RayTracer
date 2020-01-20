/* IMAGE.cpp
 *   by Lut99
 *
 * Created:
 *   1/20/2020, 3:03:07 PM
 * Last edited:
 *   1/20/2020, 5:10:54 PM
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
            char red = 255 * this->operator[](y)[x][0];
            char green = 255 * this->operator[](y)[x][1];
            char blue = 255 * this->operator[](y)[x][2];

            out << red << ' ' << green << ' ' << blue;
        }
        out << endl;
    }

    // Close the file and done
    out.close();
}