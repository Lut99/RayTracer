/* RENDERER.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:00:17 PM
 * Last edited:
 *   1/22/2020, 3:43:58 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file is the renderer for the RayTracer. For now, it is a very
 *   simple version that only does some basic tracing. This edition now
 *   also implements anti-aliasing.
**/

#include <iostream>
#include <limits>

#include "lib/include/Random.hpp"

#include "lib/include/Image.hpp"
#include "lib/include/Ray.hpp"

#include "lib/include/RenderObjectCollection.hpp"
#include "lib/include/Sphere.hpp"

#include "lib/include/Camera.hpp"

using namespace std;
using namespace RayTracer;

/* Returns a gradient along the y-axis. */
Vec3 default_colour(const Ray& r) {
    Vec3 unit = r.direction.normalize();
    double t = 0.5 * (unit.y + 1.0);
    return (1.0 - t) * Vec3(1, 1, 1) + t * Vec3(0.5, 0.7, 1.0);
}


int main(int argc, char** argv) {
    int screen_width, screen_height, number_of_rays;
    // Parse screen_width and screen_height if given
    if (argc == 4) {
        try {
            screen_width = stoi(argv[1]);
            screen_height = stoi(argv[2]);
            number_of_rays = stoi(argv[3]);
        } catch (invalid_argument& e) {
            cerr << "Image width, image height and number of rays must be positive integers" << endl;
            exit(1);
        } catch (overflow_error& e) {
            cerr << "Image width, image height or number of rays is too large; try sticking to integers instead" << endl;
            exit(1);
        }
        if (screen_width < 0 || screen_height < 0 || number_of_rays < 0) {
            cerr << "Image width, image height and number of rays must be positive integers" << endl;
            exit(1);
        }
    } else if (argc == 1) {
        screen_width = 1000;
        screen_height = 500;
        number_of_rays = 100;
    } else {
        cerr << "Usage: renderer <image_width> <image_height> <number_of_rays>" << endl;
        exit(1);
    }

    // Create a list of objects
    vector<RenderObject*> objects;
    objects.resize(2);
    objects[0] = (RenderObject*) new Sphere(Vec3(0, 0, -1), 0.5);
    objects[1] = (RenderObject*) new Sphere(Vec3(0, -100.5, -1), 100);

    // Put these in a RenderObjectCollection
    RenderObjectCollection world(objects);

    // Create the camera
    Camera cam(screen_width, screen_height);

    Image out(screen_width, screen_height);
    for (int y = screen_height-1; y >= 0; y--) {
        for (int x = 0; x < screen_width; x++) {
            Vec3 col;
            for (int r = 0; r < number_of_rays; r++) {
                double u = double(x + random_double()) / double(screen_width);
                double v = double(screen_height - 1 - y + random_double()) / double(screen_height);

                Ray ray = cam.get_ray(u, v);

                // Check if the Ray hits anything
                HitRecord hit;
                if (world.hit(ray, 0.0, numeric_limits<double>::max(), hit)) {
                    col += world.colour(hit);
                } else {
                    col += default_colour(ray);
                }
            }

            // Set the colour to the output pixel (but don't forget to average)
            out[y][x] = col / number_of_rays;
        }
    }

    // Write the image
    out.to_png("test.png");
}