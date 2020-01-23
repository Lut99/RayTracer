/* RENDERER.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:00:17 PM
 * Last edited:
 *   1/23/2020, 9:00:19 AM
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

#include "lib/include/Image.hpp"
#include "lib/include/Ray.hpp"

#include "lib/include/RenderObjectCollection.hpp"
#include "lib/include/Sphere.hpp"

#include "lib/include/Camera.hpp"
#include "lib/include/EfficientCamera.hpp"

#include "lib/include/Material.hpp"

using namespace std;
using namespace RayTracer;

int main(int argc, const char** argv) {
    unsigned int screen_width, screen_height, number_of_rays, n_threads;
    bool show_progressbar, correct_gamma, use_efficient_camera;

    screen_width = 600;
    screen_height = 300;
    number_of_rays = 500;
    show_progressbar = true;
    correct_gamma = true;

    n_threads = 4;
    use_efficient_camera = true;

    // Create a list of objects
    vector<RenderObject*> objects;
    objects.resize(4);
    objects[0] = (RenderObject*) new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.8, 0.3, 0.3)));
    objects[1] = (RenderObject*) new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0)));
    objects[2] = (RenderObject*) new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.3));
    objects[3] = (RenderObject*) new Sphere(Vec3(-1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.8, 0.8), 0.1));

    // Put these in a RenderObjectCollection
    RenderObjectCollection world(objects);

    if (use_efficient_camera) {
        // Create the camera
        EfficientCamera cam(screen_width, screen_height, number_of_rays, show_progressbar, correct_gamma, n_threads);

        Image out = cam.render(world);

        // Write the image
        out.to_png("test.png");
    } else {
        // Create the camera
        Camera cam(screen_width, screen_height, number_of_rays, show_progressbar, correct_gamma);

        Image out = cam.render(world);

        // Write the image
        out.to_png("test.png");
    }

    // CLEANUP: Deallocate all objects
    for (std::size_t i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
}