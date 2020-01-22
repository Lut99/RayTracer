/* EFFICIENT CAMERA.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 9:13:21 PM
 * Last edited:
 *   1/22/2020, 9:37:43 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The EfficientCamera class is a multi-threaded and possibly
 *   GPU-rendered version of the normal camera. This particular file is the
 *   implementation file for EfficientCamera.hpp.
**/

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
    int done;
    const RenderObjectCollection* world;
    EfficientCamera* camera;
};



EfficientCamera::EfficientCamera(int screen_width, int screen_height, int rays_per_pixel, bool correct_gamma, int num_of_threads)
    : width(screen_width),
    height(screen_height),
    rays(rays_per_pixel),
    gamma(correct_gamma),
    n_threads(num_of_threads)
{
    double ratio = (double) this->width / (double) this->height;

    this->lower_left_corner = Vec3(-2.0, -1.0, -1.0);
    this->horizontal = Vec3(4.0, 0.0, 0.0);
    this->vertical = Vec3(0.0, 4.0 / ratio, 0.0);
    this->origin = Vec3(0.0, 0.0, 0.0);
}

Ray EfficientCamera::get_ray(double u, double v) const {
    return Ray(this->origin, this->lower_left_corner + u * this->horizontal + v * this->vertical - this->origin);
}

/* Shoots a ray. If it hits something, apply diffusion. Otherwise, return the sky colour. */
bool check;
Vec3 shoot_ray(const Ray& ray, const RenderObjectCollection& world) {
    HitRecord record;
    if (world.hit(ray, 0.0, numeric_limits<double>::max(), record)) {
        check = true;
        Vec3 target = record.hitpoint + record.hitpoint.normalize() + random_in_unit_sphere();
        return 0.5 * shoot_ray(Ray(record.hitpoint, (target - record.hitpoint)), world);
    } else {
        if (!check) {
            //cout << "Found air" << endl;
        }
        Vec3 unit = ray.direction.normalize();
        double t = 0.5 * (unit.y + 1.0);
        return (1.0 - t) * Vec3(1, 1, 1) + t * Vec3(0.5, 0.7, 1.0);
    }
}

void* render_thread(void* v_args) {
    ThreadData* args = (ThreadData*) v_args;
    EfficientCamera* cam = args->camera;

    for (int y = args->row_end; y >= args->row_start; y--) {
        for (int x = 0; x < cam->width; x++) {
            Vec3 col;
            for (int r = 0; r < cam->rays; r++) {
                double u, v;
                if (cam->rays > 1) {
                    u = double(x + random_double()) / double(cam->width);
                    v = double(cam->height - 1 - y + random_double()) / double(cam->height);
                } else {
                    u = double(x) / double(cam->width);
                    v = double(cam->height - 1 - y) / double(cam->height);
                }

                Ray ray = cam->get_ray(u, v);

                // Get the colour
                check = false;
                col += shoot_ray(ray, *args->world);
            }

            // Compute the colour average
            Vec3 avg_col = col / cam->rays;
            if (cam->gamma) {
                // Gamma-correct the avg_colour
                args->out[0][y][x] = Vec3(sqrt(avg_col.x), sqrt(avg_col.y), sqrt(avg_col.z));
            } else {
                args->out[0][y][x] = avg_col;
            }
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
        threads[i].done = 0;
        threads[i].world = &world;
        threads[i].camera = this;
    }

    // Run the threads
    for (int i = 0; i < this->n_threads; i++) {
        pthread_create(&threads[i].tid, NULL, render_thread, (void*) &threads[i]);
    }

    // Wait for all of them to reap
    int all_done = 0;
    ProgressBar prgs(0, this->height);
    while (all_done < this->height) {
        // Sleep a second
        this_thread::sleep_for(chrono::milliseconds(500));

        // Check if we're done
        all_done = 0;
        for (int i = 0; i < n_threads; i++) {
            all_done += threads[i].done;
        }

        prgs.set(all_done);
    }

    // Reap 'em
    for (int i = 0; i < this->n_threads; i++) {
        pthread_join(threads[i].tid, NULL);
    }

    return out;
}