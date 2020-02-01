/* RAY TRACER.cpp
 *   by Lut99
 *
 * Created:
 *   1/31/2020, 2:00:11 PM
 * Last edited:
 *   1/31/2020, 2:18:24 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file is the entrypoint for the RayTracer. It handles collecting
 *   arguments and loading from .json files, but the actual rendering will be
 *   done by Renderer.cpp.
 * 
 * Dependencies:
 *   This project depends on the following libraries:
 *     - LodePNG.cpp, for writing to PNG files
 *       (https://lodev.org/lodepng/)
 *     - cxxopts.hpp, for parsing arguments
 *       (https://github.com/jarro2783/cxxopts)
 *     - json.hpp, for reading the scene files
 *       (https://github.com/nlohmann/json)
**/

#include <fstream>
#include <iostream>
#include <limits>
#include <chrono>
#include <cerrno>
#include <cstring>

#include "lib/include/Image.hpp"

#include "lib/include/RenderWorld.hpp"
#include "lib/include/objects/Sphere.hpp"
#include "lib/include/Camera.hpp"

#include "lib/include/materials/Lambertian.hpp"
#include "lib/include/materials/Metal.hpp"
#include "lib/include/materials/Dielectric.hpp"

#include "lib/include/ProgressBar.hpp"
#include "lib/include/cxxopts.hpp"
#include "lib/include/WorldIO.hpp"
#include "lib/include/Renderer.hpp"

using namespace std;
using namespace RayTracer;
using namespace cxxopts;

int main(int argc, char** argv) {
    std::string filename, scenename;
    unsigned int screen_width, screen_height, number_of_rays, n_threads, batch_size, vfov;
    bool show_progressbar, correct_gamma;
    double aperture;

    Options arguments("RayTracer", "Renders images using a custom-written RayTracer.");
    arguments.add_options()
        ("f,filename", "The name of the output picture (relative to the run directory)", value<string>())
        ("s,scene", "Path to the JSON file that will be used to create a RenderWorld", value<string>())
        ("W,width", "The width (in pixels) of the output image", value<unsigned int>())
        ("H,height", "The height (in pixels) of the output image", value<unsigned int>())
        ("r,rays", "The number of rays shot per pixel", value<unsigned int>())
        ("p,progressbar", "If given, shows a progressbar to indice the render process")
        ("g,gamma", "If given, corrects the gamma before saving")
        ("v,vfov", "Specifies the field of view for the camera.", value<unsigned int>())
        ("a,aperture", "Determines the aperture of the camera. Determines the amount of blur.", value<double>())
        #ifdef RENDER_THREADED
        ("t,threads", "The number of threads this program runs", value<unsigned int>())
        ("b,batch_size", "The batch size for the ThreadPool", value<unsigned int>())
        #endif
        ;
    
    auto result = arguments.parse(argc, argv);
    
    if (result.count("filename")) {
        filename = result["filename"].as<string>();
    } else {
        filename = "output/out1.png";
    }
    if (result.count("scene")) {
        scenename = result["scene"].as<string>();
    } else {
        scenename = "";
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
        number_of_rays = 500;
    } catch (OptionParseException& opt) {
        cerr << "Could not parse number of rays: " << opt.what() << endl;
        exit(-1);
    }
    try {
        vfov = result["vfov"].as<unsigned int>();
    } catch (domain_error& opt) {
        vfov = 90;
    } catch (OptionParseException& opt) {
        cerr << "Could not parse field of view: " << opt.what() << endl;
        exit(-1);
    }
    try {
        aperture = result["aperture"].as<double>();
    } catch (domain_error& opt) {
        aperture = 0.5;
    } catch (OptionParseException& opt) {
        cerr << "Could not parse aperture: " << opt.what() << endl;
        exit(-1);
    }
    show_progressbar = result.count("progressbar") != 0;
    correct_gamma = result.count("gamma") != 0;
    #ifdef RENDER_THREADED
    try {
        n_threads = result["threads"].as<unsigned int>();
    } catch (domain_error& opt) {
        n_threads = 16;
    } catch (OptionParseException& opt) {
        cerr << "Could not parse number of threads: " << opt.what() << endl;
        exit(-1);
    }
    try {
        batch_size = result["batch_size"].as<unsigned int>();
    } catch (domain_error& opt) {
        batch_size = 1000;
    } catch (OptionParseException& opt) {
        cerr << "Could not parse batch size: " << opt.what() << endl;
        exit(-1);
    }
    #endif

    cout << endl << "*** RayTracer Renderer ***" << endl << endl;
    cout << "Using options:" << endl;
    cout << "  Output file        : " << filename << endl;
    if (scenename != "") {
        cout << "  Scene used         : " << scenename << endl;
    }
    cout << "  Image dimensions   : " << screen_width << "x" << screen_height << "px" << endl;
    cout << "  Number of rays     : " << number_of_rays << endl;
    #ifdef RENDER_THREADED
    cout << "  Number of threads  : " << n_threads << endl;
    cout << "  Batch size         : " << batch_size << endl;
    #endif
    cout << "  Field of view      : " << vfov << endl;
    cout << "  Aperture           : " << aperture << endl;
    cout << "  Correct for gamma  ? ";
    if (correct_gamma) {
        cout << "yes" << endl;
    } else {
        cout << "no" << endl;
    }

    RenderWorld* world;
    Camera *cam = NULL;
    if (scenename.empty()) {
        cout << endl << "Generating world..." << endl;
        world = new RenderWorld();
        world->add_object(new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.1, 0.2, 0.5))));
        world->add_object(new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0))));
        world->add_object(new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.0)));
        world->add_object(new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(Vec3(1.0, 1.0, 1.0), 1.5)));
    } else {
        // Open a file to the input scene
        cout << endl << "Loading world..." << endl;
        ifstream scene_file(scenename);
        if (!scene_file.is_open()) {
            cerr << "Could not open file: " << strerror(errno) << endl;
            exit(-1);
        }
        nlohmann::json j;
        scene_file >> j;
        scene_file.close();

        world = WorldIO::from_json(j);

        cout << "Loaded " << world->get_object_count() << " objects" << endl;

        if (!j["camera"].is_null()) {
            cout << "Loading camera..." << endl;
            cam = WorldIO::camera_from_json(j["camera"]);
            // Set some meta variables
            cam->width = screen_width;
            cam->height = screen_height;
            cam->rays = number_of_rays;
            cam->gamma = correct_gamma;
            // Make sure the camera is up-to-date
            cam->recompute();
            // Print some nice stuff
            if (cam->vfov != vfov) {
                cout << "  Camera: overriding field of view to " << cam->vfov << endl;
            }
            if (cam->aperture != aperture) {
                cout << "  Camera: overriding aperture to " << cam->aperture << endl;
            }
        }
    }

    if (cam == NULL) {
        cout << "Creating camera..." << endl;
        cam = new Camera(Vec3(7, 2, 2), Vec3(0, 0, -1), Vec3(0, 1, 0), vfov, aperture, screen_width, screen_height, number_of_rays, correct_gamma);
    }

    // Render one picture
    cout << endl << "Rendering..." << endl;
    Renderer renderer(n_threads, batch_size, show_progressbar);

    auto start = chrono::system_clock::now();

    Image out = renderer.render(world, cam);

    auto stop = chrono::system_clock::now();
    auto duration = stop - start;
    auto duration_ms = chrono::duration_cast<chrono::milliseconds>(duration);

    cout << "Done, rendered frame in " << ((double) duration_ms.count()) / 1000.0 << " seconds" << endl;

    // Delete the allocated objects
    delete world;
    delete cam;

    // Write the image
    cout << "Writing output..." << endl;
    out.to_png(filename);

    cout << endl <<  "Done." << endl << endl;
}