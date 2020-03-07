/* RAY TRACER.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:00:17 PM
 * Last edited:
 *   2/9/2020, 5:42:30 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file is the renderer for the RayTracer. This new version includes the
 *   complete 'Raytracing in One Weekend' tutorial, but then multithreaded (if
 *   compiled that way). Additionally, it will also be able to handle moving
 *   objects and rendering a movie. This particular file is the implementation
 *   file for Renderer.hpp.
**/

#include "Config.hpp"

#ifdef THREADED
#include "ThreadPool.hpp"
#endif
#include "ProgressBar.hpp"
#include "Renderer.hpp"

using namespace std;
using namespace RayTracer;


Renderer::Renderer(int num_of_threads, int batch_size, bool show_progressbar)
    : n_threads(num_of_threads),
    batch_size(batch_size),
    show_progressbar(show_progressbar)
{}

Image Renderer::render(RenderWorld* world, Camera* cam) {
    Image out(cam->width, cam->height);
    ProgressBar prgrs(0, cam->width * cam->height - 1);

    // Render it (how depends on if the program is threaded or not)
    #ifndef THREADED

    for (int y = cam->height-1; y >= 0; y--) {
        for (int x = 0; x < cam->width; x++) {
            // Render the pixel
            out[y][x] = world->render_pixel(x, y, *cam);

            if (this->show_progressbar) {
                prgrs.update();
            }
        }
    }

    #else

    // Create a thread pool
    ThreadPool pool(this->n_threads, this->batch_size);

    // Use the thread pool to render it
    unsigned long batch_index = 0;
    unsigned long to_do = cam->width * cam->height;
    while (batch_index < to_do) {
        // Update the progressbar in any case for smoothness
        if (this->show_progressbar) {
            prgrs.set(batch_index);
        }

        // If a the queue is full, wait for another thread to notify us it has taken an element
        if (pool.batch_queue_full()) {
            pool.wait();
        }

        // Create a new batch and append it
        PixelBatch batch = pool.get_batch(cam->width, cam->height, batch_index);
        batch.camera = cam;
        batch.world = world;
        batch.out = &out;
        pool.add_batch(batch);
    }

    // Wait until all threads have been reaped
    pool.complete();
    pool.stop();

    // Update the bar one last time
    if (this->show_progressbar) {
        prgrs.set(batch_index);
    }

    #endif

    return out;
}

void Renderer::render_animation(RenderWorld* world, Camera* cam, Frames& out) {
    #ifdef THREADED
    ThreadPool pool(this->n_threads, this->batch_size);
    #endif

    for (std::size_t i = 0; i < out.n_frames; i++) {
        // Render this frame
        ProgressBar prgrs(0, cam->width * cam->height - 1, "(" + to_string(i + 1) + "/" + to_string(out.n_frames) + ")", "");

        #ifndef THREADED

        for (int y = cam->height-1; y >= 0; y--) {
            for (int x = 0; x < cam->width; x++) {
                // Render the pixel
                out[y][x] = world->render_pixel(x, y, *cam);

                if (this->show_progressbar) {
                    prgrs.update();
                }
            }
        }

        #else

        // Use the thread pool to render it
        unsigned long batch_index = 0;
        unsigned long to_do = cam->width * cam->height;
        while (batch_index < to_do) {
            // Update the progressbar in any case for smoothness
            if (this->show_progressbar) {
                prgrs.set(batch_index);
            }

            // If a the queue is full, continue
            if (pool.batch_queue_full()) {
                pool.wait();
            }

            // Create a new batch and append it
            PixelBatch batch = pool.get_batch(cam->width, cam->height, batch_index);
            batch.camera = cam;
            batch.world = world;
            batch.out = &out.get_current_frame();
            pool.add_batch(batch);
        }

        // Make sure the threads are complete
        pool.complete();

        #endif

        // Update the frames
        out.next();

        // Update the world & camera
        chrono::milliseconds time_passed(1000 / out.fps);
        world->update(time_passed);
        cam->update(time_passed);
    }

    #ifdef THREADED
    pool.stop();
    #endif
}