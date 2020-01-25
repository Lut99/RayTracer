/* RENDERER.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:00:17 PM
 * Last edited:
 *   1/25/2020, 1:04:50 PM
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

#include "lib/include/Material.hpp"

#include "lib/include/cxxopts.hpp"

using namespace std;
using namespace RayTracer;
using namespace cxxopts;

int main(int argc, char** argv) {
    std::string filename;
    unsigned int screen_width, screen_height, number_of_rays, n_threads;
    bool show_progressbar, correct_gamma;

    Options arguments("RayTracer", "Renders images using a custom-written RayTracer.");
    arguments.add_options()
        ("f,filename", "The name of the output picture (relative to the run directory)", value<string>())
        ("W,width", "The width (in pixels) of the output image", value<unsigned int>())
        ("H,height", "The height (in pixels) of the output image", value<unsigned int>())
        ("r,rays", "The number of rays shot per pixel", value<unsigned int>())
        ("p,progressbar", "If given, shows a progressbar to indice the render process")
        ("g,gamma", "If given, corrects the gamme before saving")
        ;
    
    auto result = arguments.parse(argc, argv);
    
    if (result.count("filename")) {
        filename = result["filename"].as<string>();
    } else {
        filename = "test.png";
    }
    try {
        screen_width = result["width"].as<unsigned int>();
    } catch (domain_error& opt) {
        screen_width = 1000;
    } catch (OptionParseException& opt) {
        cerr << "Could not parse image width: " << opt.what() << endl;
        exit(-1);
    }
    try {
        screen_height = result["height"].as<unsigned int>();
    } catch (domain_error& opt) {
        screen_height = 500;
    } catch (OptionParseException& opt) {
        cerr << "Could not parse image height: " << opt.what() << endl;
        exit(-1);
    }
    try {
        number_of_rays = result["rays"].as<unsigned int>();
    } catch (domain_error& opt) {
        number_of_rays = 300;
    } catch (OptionParseException& opt) {
        cerr << "Could not parse number of rays: " << opt.what() << endl;
        exit(-1);
    }
    show_progressbar = result.count("progressbar") != 0;
    correct_gamma = result.count("gamma") != 0;

    cout << endl << "*** RayTracer Renderer ***" << endl << endl;
    cout << "Using options:" << endl;
    cout << "  Output file      : " << filename << endl;
    cout << "  Image dimensions : " << screen_width << "x" << screen_height << "px" << endl;
    cout << "  Number of rays   : " << number_of_rays << endl;
    #ifdef CAMERA_THREADS
    cout << "  Number of threads: " << CAMERA_THREADS << endl;
    #endif
    cout << "  Correct for gamma? ";
    if (correct_gamma) {
        cout << "yes" << endl;
    } else {
        cout << "no" << endl;
    }
    cout << endl << "Rendering..." << endl;

    // Create a list of objects
    vector<RenderObject*> objects;
    objects.resize(4);
    objects[0] = (RenderObject*) new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.8, 0.3, 0.3)));
    objects[1] = (RenderObject*) new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0)));
    objects[2] = (RenderObject*) new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.6, 0.6, 0.6), 0.3));
    objects[3] = (RenderObject*) new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(Vec3(1.0, 1.0, 1.0), 1.5));

    // Put these in a RenderObjectCollection
    RenderObjectCollection world(objects);

    // Initialize the camera
    Camera cam(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0), 90, screen_width, screen_height, number_of_rays, show_progressbar, correct_gamma);

    // Render one picture
    Image out = cam.render(world);

    // Write the image
    out.to_png(filename);

    // CLEANUP: Deallocate all objects
    for (std::size_t i = 0; i < objects.size(); i++) {
        delete objects[i];
    }

    cout << endl <<  "Done." << endl << endl;
}