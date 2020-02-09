/* IMAGE.hpp
 *   by Lut99
 *
 * Created:
 *   1/20/2020, 3:01:57 PM
 * Last edited:
 *   2/9/2020, 1:01:17 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Image class is meant to be raw image storage. It is not much more
 *   than a wrapped 3D-array (that is allocated in 1D) with the added bonus
 *   of easy to-file features. This particular file is the header file for
 *   Image.cpp.
**/

#ifndef IMAGE_H
#define IMAGE_H

#include <memory>
#include <string>

#include "Vec3.hpp"

namespace RayTracer {
    class Pixel {
        private:
            double* data;
        public:
            double& r();
            double& g();
            double& b();

            /* The pixel class is used to access a pixel in an Image. Should not be used manually. Note that the given pointer is not deallocated. */
            Pixel(double* colours);

            double& operator[](unsigned int index);
            Pixel& operator=(const Pixel& other);
            Pixel& operator=(const Vec3& other);
    };

    class ImageRow {
        private:
            /* The internal representation of the data. This is aligned as a row of three colour channels. */
            double* data;
        public:
            const unsigned int width;

            /* The ImageRow class is only used for operator[] access in a complete Image. Note: Does not deallocate given pointer */
            ImageRow(double* data, unsigned int width);
            
            Pixel operator[](unsigned int index);
    };

    class Image {
        private:
            /* The internal representation of the data. This is aligned as rows, then columns and then three colour values. */
            double *data;
        public:
            const unsigned int width;
            const unsigned int height;

            /* The Image class is meant to be raw image storage. It is not much more than a wrapped 3D-array (that is allocated in 1D) with the added bonus of easy to-file features. */
            Image(unsigned int width, unsigned int height);
            /* Copy constructor for the Image Class. */
            Image(const Image& other);
            /* Move constructor for the Image Class. */
            Image(Image&& other);
            ~Image();

            /* Provides access to a pixel inside the array (skipping ImageRows). */
            Pixel pixel(unsigned int row, unsigned int col);

            /* Provides array subscript access to a row inside the Image (which is another image) */
            ImageRow operator[](unsigned int index);

            /* Saves the image to a .ppm image file. */
            void to_png(std::string path);
    };
}

#endif