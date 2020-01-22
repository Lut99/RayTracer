/* RENDERER.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:00:17 PM
 * Last edited:
 *   1/22/2020, 1:54:17 PM
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
#include "lib/include/Sphere.hpp"

#include <iostream>

using namespace std;
using namespace RayTracer;

/* Returns a gradient along the y-axis. */
Vec3 default_colour(const Ray& r) {
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

    Sphere obj1(Vec3(0, 0, -1), 0.5);

    Image out(screen_width, screen_height);
    for (int y = screen_height-1; y >= 0; y--) {
        for (int x = 0; x < screen_width; x++) {
            double u = double(x) / double(screen_width);
            double v = double(screen_height - 1 - y) / double(screen_height);

            Ray r(origin, lower_left_corner + u*horizontal + v*vertical);

            // Check if the Ray hits anything
            Vec3 col;
            if (obj1.hit(r) != 0.0) {
                // Draw that pixel
                col = obj1.colour(Vec3(0, 0, 0));
            } else {
                col = default_colour(r);
            }

            // Set the colour to the output pixel
            out[y][x] = col;
        }
    }

    // Write the image
    out.to_png("test.png");
}