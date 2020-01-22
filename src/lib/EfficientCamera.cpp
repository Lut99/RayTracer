/* EFFICIENT CAMERA.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 9:13:21 PM
 * Last edited:
 *   1/22/2020, 10:10:55 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The EfficientCamera class is a multi-threaded and possibly
 *   GPU-rendered version of the normal camera. This particular file is the
 *   implementation file for EfficientCamera.hpp.
**/

#include <atomic>
#include <thread>
#include <pthread.h>
#include <iostream>

#include "include/ProgressBar.hpp"
#include "include/Random.hpp"
#include "include/EfficientCamera.hpp"

using namespace std;
using namespace RayTracer;


struct ThreadData {
    int row_start;
    int row_end;
    pthread_t tid;
    Image* out;
    atomic<int> done;
    const RenderObjectCollection* world;
    EfficientCamera* camera;
};



EfficientCamera::EfficientCamera(int screen_width, int screen_height, int rays_per_pixel, bool show_progressbar, bool correct_gamma, int num_of_threads)
    : Camera(screen_width, screen_height, rays_per_pixel, show_progressbar, correct_gamma),
    n_threads(num_of_threads)
{}

void* render_thread(void* v_args) {
    ThreadData* args = (ThreadData*) v_args;
    EfficientCamera* cam = args->camera;

    for (int y = args->row_end; y >= args->row_start; y--) {
        for (int x = 0; x < cam->width; x++) {
            args->out[0][y][x] = args->camera->render_pixel(x, y, *args->world);

            args->done.store(args->done.load() + 1);
        }

        // Save the picture very y
        //out.to_png("output/test_" + to_string(y) + ".png");
    }
}

Image EfficientCamera::render(const RenderObjectCollection& world) {
    Image out(this->width, this->height);

    // Prepare the structs for the threads
    int thread_n_rows = this->height / n_threads;
    ThreadData threads[this->n_threads];
    for (int i = 0; i < n_threads; i++) {
        threads[i].row_start = i * thread_n_rows;
        threads[i].row_end = threads[i].row_start + thread_n_rows - 1;
        if (i == n_threads - 1) {
            // Give the last thread the rest
            threads[i].row_end = this->height - 1;
        }
        threads[i].out = &out;
        threads[i].done.store(0);
        threads[i].world = &world;
        threads[i].camera = this;
    }

    // Run the threads
    for (int i = 0; i < this->n_threads; i++) {
        pthread_create(&threads[i].tid, NULL, render_thread, (void*) &threads[i]);
    }

    // Wait for all of them to reap
    int all_done = 0;
    ProgressBar prgs(0, this->height * this->width);
    while (all_done < this->height * this->width) {
        // Sleep a second
        this_thread::sleep_for(chrono::milliseconds(500));

        // Check if we're done
        all_done = 0;
        for (int i = 0; i < n_threads; i++) {
            all_done += threads[i].done.load();
        }

        if (this->progress) {
            prgs.set(all_done);
        }
    }

    // Reap 'em
    for (int i = 0; i < this->n_threads; i++) {
        pthread_join(threads[i].tid, NULL);
    }

    return out;
}