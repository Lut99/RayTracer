/* RENDERER.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:00:17 PM
 * Last edited:
 *   1/22/2020, 6:16:38 PM
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

using namespace std;
using namespace RayTracer;

int main(int argc, const char** argv) {
    unsigned int screen_width, screen_height, number_of_rays;
    bool show_progressbar, correct_gamma;

    screen_width = 200;
    screen_height = 100;
    number_of_rays = 1000;
    show_progressbar = true;
    correct_gamma = false;

    // Create the camera
    Camera cam(screen_width, screen_height, number_of_rays, show_progressbar, correct_gamma);

    // Create a list of objects
    vector<RenderObject*> objects;
    objects.resize(2);
    objects[0] = (RenderObject*) new Sphere(Vec3(0, 0, -1), 0.5);
    objects[1] = (RenderObject*) new Sphere(Vec3(0, -100.5, -1), 100);

    // Put these in a RenderObjectCollection
    RenderObjectCollection world(objects);

    Image out = cam.render(world);

    // Write the image
    out.to_png("test.png");

    // CLEANUP: Deallocate all objects
    for (std::size_t i = 0; i < objects.size(); i++) {
        delete objects[i];
    }
}