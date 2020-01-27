/* RENDERER.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:00:17 PM
 * Last edited:
 *   1/27/2020, 3:22:40 PM
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
#include <chrono>

#include "lib/include/Image.hpp"

#include "lib/include/RenderWorld.hpp"
#include "lib/include/Sphere.hpp"
#include "lib/include/Camera.hpp"
#include "lib/include/Material.hpp"

#include "lib/include/ProgressBar.hpp"
#include "lib/include/cxxopts.hpp"

using namespace std;
using namespace RayTracer;
using namespace cxxopts;

int main(int argc, char** argv) {
    std::string filename;
    unsigned int screen_width, screen_height, number_of_rays, n_threads, batch_size;
    bool show_progressbar, correct_gamma;

    Options arguments("RayTracer", "Renders images using a custom-written RayTracer.");
    arguments.add_options()
        ("f,filename", "The name of the output picture (relative to the run directory)", value<string>())
        ("W,width", "The width (in pixels) of the output image", value<unsigned int>())
        ("H,height", "The height (in pixels) of the output image", value<unsigned int>())
        ("r,rays", "The number of rays shot per pixel", value<unsigned int>())
        ("p,progressbar", "If given, shows a progressbar to indice the render process")
        ("g,gamma", "If given, corrects the gamme before saving")
        #ifdef RENDER_THREADED
        ("t,threads", "The number of threads this program runs")
        ("b,batch_size", "The batch size for the ThreadPool")
        #endif
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
        batch_size = 500;
    } catch (OptionParseException& opt) {
        cerr << "Could not parse batch size: " << opt.what() << endl;
        exit(-1);
    }
    #endif

    cout << endl << "*** RayTracer Renderer ***" << endl << endl;
    cout << "Using options:" << endl;
    cout << "  Output file        : " << filename << endl;
    cout << "  Image dimensions   : " << screen_width << "x" << screen_height << "px" << endl;
    cout << "  Number of rays     : " << number_of_rays << endl;
    #ifdef RENDER_THREADED
    cout << "  Number of threads  : " << n_threads << endl;
    cout << "  Batch size         : " << batch_size << endl;
    #endif
    cout << "  Correct for gamma  ? ";
    if (correct_gamma) {
        cout << "yes" << endl;
    } else {
        cout << "no" << endl;
    }
    cout << "  Showing progressbar? ";
    if (show_progressbar) {
        cout << "yes" << endl;
    } else {
        cout << "no" << endl;
    }

    cout << endl << "Creating world..." << endl;
    RenderWorld *world = new RenderWorld();

    cout << "Appending objects..." << endl;
    world->add_object(new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.8, 0.3, 0.3))));
    world->add_object(new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.8, 0.8, 0.0))));
    world->add_object(new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.6, 0.6, 0.6), 0.3)));
    world->add_object(new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(Vec3(1.0, 1.0, 1.0), 1.5)));

    cout << "Appending cameras..." << endl;
    world->add_camera(new Camera(Vec3(0, 0, 0), Vec3(0, 0, -1), Vec3(0, 1, 0), 90, screen_width, screen_height, number_of_rays, correct_gamma));

    // Render one picture
    cout << endl << "Rendering..." << endl;

    Image out(screen_width, screen_height);
    ProgressBar prgrs(0, screen_width * screen_height - 1);
    auto start = chrono::system_clock::now();

    // Render it (how depends on if the program is threaded or not)
    #ifndef RENDER_THREADED

    for (int y = screen_height-1; y >= 0; y--) {
        for (int x = 0; x < screen_width; x++) {
            // Render the pixel
            out[y][x] = world->render_pixel(x, y);

            if (show_progressbar) {
                prgrs.update();
            }
        }
    }

    #else

    // Use the thread pool to render it
    unsigned long batch_index = 0;
    unsigned long to_do = this->width * this->height;
    cout << "Starting render. Batch_index=" << batch_index << ", to_do=" << to_do << endl;
    while (batch_index < to_do) {
        // If a the queue is full, continue
        if (this->pool->batch_queue_full()) {
            continue;
        }

        // Create a new batch and append it
        PixelBatch batch = this->pool->get_batch(batch_index);
        batch.world = &world;
        batch.out = &out;
        this->pool->add_batch(batch);

        // Update the progressbar
        if (this->progress) {
            prgrs.set(batch_index);
        }
    }

    // Wait until all threads have been reaped
    this->pool->stop();
    cout << "Main stopped." << endl;

    #endif

    auto stop = chrono::system_clock::now();
    auto duration = stop - start;
    auto duration_ms = chrono::duration_cast<chrono::milliseconds>(duration);

    cout << "Done, rendered frame in " << ((double) duration_ms.count()) / 1000.0 << " seconds" << endl;

    // Write the image
    cout << "Writing output..." << endl;
    out.to_png(filename);

    // CLEANUP: Deallocate the world
    delete world;

    cout << endl <<  "Done." << endl << endl;
}