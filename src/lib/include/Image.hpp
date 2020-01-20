/* IMAGE.hpp
 *   by Lut99
 *
 * Created:
 *   1/20/2020, 3:01:57 PM
 * Last edited:
 *   1/20/2020, 3:26:37 PM
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

#include <cstddef>
#include <string>

namespace RayTracer {
    class Image {
        private:
            /* The internal representation of the data. This is aligned as rows, then columns and then three colour values. */
            double *data;
        public:
            const std::size_t width;
            const std::size_t height;

            /* The Image class is meant to be raw image storage. It is not much more than a wrapped 3D-array (that is allocated in 1D) with the added bonus of easy to-file features. */
            Image(std::size_t width, std::size_t height);
            ~Image();

            /* Provides access to a row inside the array. */
            double& pixel(std::size_t x, std::size_t y, std::size_t z);

            /* Saves the image to a .ppm image file. */
            void to_ppm(std::string path);
    };
}

#endif