/* RENDERER.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:00:17 PM
 * Last edited:
 *   1/22/2020, 4:03:04 PM
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
    Camera cam(screen_width, screen_height, number_of_rays);

    Image out = cam.render(world);

    // Write the image
    out.to_png("test.png");
}