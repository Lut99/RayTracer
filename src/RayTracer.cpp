/* RAY TRACER.cpp
 *   by Lut99
 *
 * Created:
 *   1/31/2020, 2:00:11 PM
 * Last edited:
 *   07/03/2020, 12:56:31
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

#include "Config.hpp"

#include "Image.hpp"

#include "RenderWorld.hpp"
#include "Sphere.hpp"
#include "Camera.hpp"

#include "Lambertian.hpp"
#include "Metal.hpp"
#include "Dielectric.hpp"

#include "CameraRotation.hpp"

#include "ProgressBar.hpp"
#include "cxxopts.hpp"
#include "Renderer.hpp"

using namespace std;
using namespace RayTracer;
using namespace cxxopts;


/* Creates the default scene of three spheres. */
RenderWorld* create_default_renderworld() {
    RenderWorld* world = new RenderWorld();
    world->add_object(new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.1, 0.2, 0.5))));
    world->add_object(new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0))));
    world->add_object(new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.0)));
    world->add_object(new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(Vec3(1.0, 1.0, 1.0), 1.5)));

    return world;
}

/* Updates a camera object so that it knows the width, height, number of rays and whether it should correct for gamma. */
void update_camera(Camera* cam, unsigned int screen_width, unsigned int screen_height, unsigned int number_of_rays, bool correct_gamma) {
    cam->width = screen_width;
    cam->height = screen_height;
    cam->rays = number_of_rays;
    cam->gamma = correct_gamma;
}
/* Creates a new camera for the default scene. */
Camera* create_default_camera(double vfov, double aperture, unsigned int screen_width, unsigned int screen_height, unsigned int number_of_rays, bool correct_gamma) {
    Camera* cam = new Camera(Vec3(3, 2, 2), Vec3(0, 0, -1), Vec3(0, 1, 0), vfov, aperture);
    update_camera(cam, screen_width, screen_height, number_of_rays, correct_gamma);
    cam->recompute();

    return cam;
}


int main(int argc, char** argv) {
    std::string filename, scenename, temppath;
    unsigned int screen_width, screen_height, number_of_rays, n_threads, batch_size, vfov, fps, n_frames;
    bool show_progressbar, correct_gamma, dynamic_writing;
    double aperture;

    Options arguments("RayTracer", "Renders images using a custom-written RayTracer.");
    arguments.add_options()
        ("f,filename", "The name of the output picture (relative to the run directory). If more than one frames are rendered, this will be an .mp4.", value<string>())
        ("s,scene", "Path to the JSON file that will be used to create a RenderWorld", value<string>())
        ("T,temppath", "Path to a folder where all the frames will be saved before creating an .mp4", value<string>())
        ("W,width", "The width (in pixels) of the output image", value<unsigned int>())
        ("H,height", "The height (in pixels) of the output image", value<unsigned int>())
        ("r,rays", "The number of rays shot per pixel", value<unsigned int>())
        ("v,vfov", "Specifies the field of view for the camera.", value<unsigned int>())
        ("a,aperture", "Determines the aperture of the camera. Determines the amount of blur.", value<double>())
        ("n,n_frames", "The number of frames that will be rendered. If this is higher than one, then a .mp4 is created.", value<unsigned int>())
        ("F,framerate", "The framerate of the output video.", value<unsigned int>())
        ("d,dynamic_write", "If given, writes the frame immediately to disk when rendered.")
        ("p,progressbar", "If given, shows a progressbar to indice the render process")
        ("g,gamma", "If given, corrects the gamma before saving")
        #ifdef THREADED
        ("t,threads", "The number of threads this program runs", value<unsigned int>())
        ("b,batch_size", "The batch size for the ThreadPool", value<unsigned int>())
        #endif
        ;
    
    auto result = arguments.parse(argc, argv);
    
    if (result.count("filename")) {
        filename = result["filename"].as<string>();
    } else {
        filename = "output/out1.";
    }
    if (result.count("scene")) {
        scenename = result["scene"].as<string>();
    } else {
        scenename = "";
    }
    if (result.count("temppath")) {
        temppath = result["temppath"].as<string>();
    } else {
        temppath = "output/animation";
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
        fps = result["framerate"].as<unsigned int>();
    } catch (domain_error& opt) {
        fps = 30;
    } catch (OptionParseException& opt) {
        cerr << "Could not parse framerate: " << opt.what() << endl;
        exit(-1);
    }
    try {
        n_frames = result["n_frames"].as<unsigned int>();
    } catch (domain_error& opt) {
        n_frames = 1;
    } catch (OptionParseException& opt) {
        cerr << "Could not parse number of frames: " << opt.what() << endl;
        exit(-1);
    }
    try {
        aperture = result["aperture"].as<double>();
    } catch (domain_error& opt) {
        aperture = 0.1;
    } catch (OptionParseException& opt) {
        cerr << "Could not parse aperture: " << opt.what() << endl;
        exit(-1);
    }
    show_progressbar = result.count("progressbar") != 0;
    correct_gamma = result.count("gamma") != 0;
    dynamic_writing = result.count("dynamic_write") != 0;
    #ifdef THREADED
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
    #else
    n_threads = 16;
    batch_size = 1000;
    #endif

    // Fix the output file if needed
    if (filename[filename.length() - 1] == '.') {
        if (n_frames == 1) {
            filename += "png";
        } else {
            filename += "mp4";
        }
    }

    cout << endl << "*** RayTracer Renderer ***" << endl << endl;
    cout << "Using options:" << endl;
    cout << "  Output file         : " << filename << endl;
    if (scenename != "") {
        cout << "  Scene used          : " << scenename << endl;
    }
    if (n_frames > 1) {
        cout << "  Temp output dict    : " << temppath << endl;
    }
    cout << "  Image dimensions    : " << screen_width << "x" << screen_height << "px" << endl;
    cout << "  Number of frames    : " << n_frames << endl;
    if (n_frames > 1) {
        cout << "  Framerate           : " << fps << endl;
    }
    cout << "  Number of rays      : " << number_of_rays << endl;
    #ifdef THREADED
    cout << "  Number of threads   : " << n_threads << endl;
    cout << "  Batch size          : " << batch_size << endl;
    #endif
    cout << "  Field of view       : " << vfov << endl;
    cout << "  Aperture            : " << aperture << endl;
    cout << "  Correct for gamma   ? ";
    if (correct_gamma) {
        cout << "yes" << endl;
    } else {
        cout << "no" << endl;
    }
    if (n_frames > 1) {
        cout << "  Write frames dynamic? ";
        if (dynamic_writing) {
            cout << "yes" << endl;
        } else {
            cout << "no" << endl;
        }
    }

    RenderWorld* world;
    Camera *cam = NULL;
    if (scenename.empty()) {
        cout << endl << "Creating default scene..." << endl;
        cout << "  Generating world..." << endl;
        world = create_default_renderworld();
        cout << "  Done, counting " << world->get_object_count() << " objects" << endl;

        cout << "  Generating camera..." << endl;
        cam = create_default_camera(vfov, aperture, screen_width, screen_height, number_of_rays, correct_gamma);
        cout << "  Done" << endl;

        cout << "Done" << endl;
    } else {
        // Open a filestream to the input scene
        cout << endl << "Loading JSON..." << endl;
        ifstream scene_file(scenename);
        if (!scene_file.is_open()) {
            cerr << "Could not open file: " << strerror(errno) << endl;
            exit(-1);
        }

        // Load a JSON from it
        nlohmann::json j;
        scene_file >> j;
        scene_file.close();

        // Check if the j is an object
        if (!j.is_object()) {
            cerr << "JSON in file \"" << scenename << "\" is not a JSON object" << endl;
            exit(-1);
        }

        // Load all the objects (if present)
        if (!j["world"].is_null()) {
            cout << "  Loading world..." << endl;
            world = RenderWorld::from_json(j["world"]);
        } else {
            cout << "  Generating world..." << endl;
            world = create_default_renderworld();
        }
        cout << "  Done, counting " << world->get_object_count() << " objects" << endl;

        // Load any camera
        if (!j["camera"].is_null()) {
            cout << "  Loading camera..." << endl;

            // Load the actual object
            cam = Camera::from_json(j["camera"]);

            // Update it to make sure it has some meta variables
            update_camera(cam, screen_width, screen_height, number_of_rays, correct_gamma);
            
            // Let the camera compute some initial variables
            cam->recompute();

            // Print some nice stuff
            if (cam->vfov != vfov) {
                cout << "    Camera: overriding field of view to " << cam->vfov << endl;
            }
            if (cam->aperture != aperture) {
                cout << "    Camera: overriding aperture to " << cam->aperture << endl;
            }
        } else {
            cam = create_default_camera(vfov, aperture, screen_width, screen_height, number_of_rays, correct_gamma);
        }
        cout << "  Done" << endl;

        cout << "Done" << endl;
    }


    // Render one picture
    cout << endl << "Rendering..." << endl;
    Renderer renderer(n_threads, batch_size, show_progressbar);

    auto start = chrono::system_clock::now();
    auto stop = start;

    if (n_frames == 1) {
        Image out = renderer.render(world, cam);

        stop = chrono::system_clock::now();

        cout << "Writing output..." << endl;
        out.to_png(filename);
    } else {
        Frames out(cam->width, cam->height, n_frames, fps, temppath, dynamic_writing);
        renderer.render_animation(world, cam, out);

        stop = chrono::system_clock::now();

        cout << "Creating .mp4..." << endl;
        out.to_mp4(filename);
    }
    auto duration = stop - start;
    auto duration_ms = chrono::duration_cast<chrono::milliseconds>(duration);

    cout << "Done, rendering took " << ((double) duration_ms.count()) / 1000.0 << " seconds" << endl;

    // Delete the allocated objects
    delete world;
    delete cam;

    cout << endl <<  "Done." << endl << endl;
}