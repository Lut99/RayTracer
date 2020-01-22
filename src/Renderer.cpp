/* RENDERER.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:00:17 PM
 * Last edited:
 *   1/22/2020, 1:29:19 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file is the renderer for the RayTracer. For now, it is a very
 *   simple version that only does some basic tracing. Later on, it will be
 *   more extensive.
**/

#include "lib/include/Image.hpp"
#include "lib/include/Ray.hpp"

#include <iostream>

using namespace std;
using namespace RayTracer;

/* Returns a gradient along a distance. */
Vec3 colour(const Ray& r) {
    // Default background colour
    Vec3 unit = r.direction.normalize();
    float t = 0.5 * (unit.y + 1.0);
    return (1.0 - t) * Vec3(1, 1, 1) + t * Vec3(0.5, 0.7, 1.0);
}


int main() {
    int screen_width, screen_height;
    screen_width = 200;
    screen_height = 100;

    Vec3 lower_left_corner(-2, -1, -1);
    Vec3 horizontal(4, 0, 0);
    Vec3 vertical(0, 2, 0);
    Vec3 origin(0, 0, 0);

    Image out(screen_width, screen_height);
    for (int y = screen_height-1; y >= 0; y--) {
        for (int x = 0; x < screen_width; x++) {
            double u = double(x) / double(screen_width);
            double v = double(screen_height - 1 - y) / double(screen_height);

            Ray r(origin, lower_left_corner + u*horizontal + v*vertical);
            Vec3 col = colour(r);

            // Set the colour to the output pixel
            out[y][x] = col;
        }
    }

    // Write the image
    out.to_png("test.png");
}