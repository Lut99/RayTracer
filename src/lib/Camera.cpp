/* CAMERA.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 3:23:14 PM
 * Last edited:
 *   1/24/2020, 11:45:23 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Camera class acts as the viewport, and therefore holds information
 *   about the viewport's location and rotation. Additionally, it handles
 *   the anti-aliasing. This particular file is the implementation file for
 *   Camera.hpp.
**/

#ifdef CAMERA_MULTITHREADED
#include <atomic>
#include <thread>
#include <pthread.h>
#endif
#include <iostream>

#include "include/ProgressBar.hpp"
#include "include/Random.hpp"
#include "include/Camera.hpp"

using namespace std;
using namespace RayTracer;


/* Define, if needed, multithread helpers */
#ifdef CAMERA_MULTITHREAD

struct ThreadData {
    int row_start;
    int row_end;
    pthread_t tid;
    Image* out;
    atomic<int> done;
    const RenderObject* world;
    const Camera* camera;
};

void* render_thread(void* v_args) {
    ThreadData* args = (ThreadData*) v_args;
    const Camera* cam = args->camera;

    for (int y = args->row_end; y >= args->row_start; y--) {
        for (int x = 0; x < cam->width; x++) {
            args->out[0][y][x] = args->camera->render_pixel(x, y, *args->world);

            args->done.store(args->done.load() + 1);
        }

        // Save the picture very y
        //out.to_png("output/test_" + to_string(y) + ".png");
    }
}

#endif


#ifndef CAMERA_MULTITHREADED
Camera::Camera(Vec3 lookfrom, Vec3 lookat, Vec3 up, double vfov, int screen_width, int screen_height, int rays_per_pixel, bool show_progressbar, bool correct_gamma)
    : width(screen_width),
    height(screen_height),
    rays(rays_per_pixel),
    progress(show_progressbar),
    gamma(correct_gamma)
{
#else
Camera::Camera(Vec3 lookfrom, Vec3 lookat, Vec3 up, double vfov, int screen_width, int screen_height, int rays_per_pixel, bool show_progressbar, bool correct_gamma, int num_of_threads)
    : width(screen_width),
    height(screen_height),
    rays(rays_per_pixel),
    progress(show_progressbar),
    gamma(correct_gamma),
    n_threads(num_of_threads)
{
#endif
    double ratio = (double) this->width / (double) this->height;

    double theta = vfov * M_PI / 180;
    double half_height = tan(theta/2);
    double half_width = ratio * half_height;

    this->origin = lookfrom;
    Vec3 w = (lookfrom - lookat).normalize();
    Vec3 u = cross(up, w).normalize();
    Vec3 v = cross(w, u);

    this->lower_left_corner = origin - half_width * u - half_height * v - w;
    this->horizontal = 2 * half_width * u;
    this->vertical = 2 * half_height * v;
}

Ray Camera::get_ray(double u, double v) const {
    return Ray(this->origin, this->lower_left_corner + u * this->horizontal + v * this->vertical - this->origin);
}

/* Shoots a ray. If it hits something, apply diffusion. Otherwise, return the sky colour. */
Vec3 Camera::shoot_ray(const Ray& ray, const RenderObject& world, int depth) const {
    HitRecord record;
    if (world.hit(ray, 0.0, numeric_limits<double>::max(), record)) {
        Ray scattered;
        Vec3 attenuation;
        if (depth < 50 && record.material->scatter(ray, record, attenuation, scattered)) {
            return attenuation * shoot_ray(scattered, world, depth + 1);
        } else {
            return Vec3(0, 0, 0);
        }
    } else {
        Vec3 unit = ray.direction.normalize();
        double t = 0.5 * (unit.y + 1.0);
        return (1.0 - t) * Vec3(1, 1, 1) + t * Vec3(0.5, 0.7, 1.0);
    }
}

Image Camera::render(const RenderObject& world) const {
    Image out(this->width, this->height);
    
    #ifndef CAMERA_MULTITHREADED

    ProgressBar prgrs(0, this->width * this->height - 1);
    for (int y = this->height-1; y >= 0; y--) {
        for (int x = 0; x < this->width; x++) {
            // Render the pixel
            out[y][x] = this->render_pixel(x, y, world);

            if (this->progress) {
                prgrs.update();
            }
        }

        // Save the picture very y
        //out.to_png("output/test_" + to_string(y) + ".png");
    }

    #else

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

    #endif

    return out;
}

Vec3 Camera::render_pixel(int x, int y, const RenderObject& world) const {
    Vec3 col;
    for (int r = 0; r < this->rays; r++) {
        double u, v;
        if (this->rays > 1) {
            u = double(x + random_double()) / double(this->width);
            v = double(this->height - 1 - y + random_double()) / double(this->height);
        } else {
            u = double(x) / double(this->width);
            v = double(this->height - 1 - y) / double(this->height);
        }

        Ray ray = this->get_ray(u, v);

        // Get the colour
        col += this->shoot_ray(ray, world);
    }

    // Compute the colour average
    Vec3 avg_col = col / this->rays;
    if (this->gamma) {
        // Gamma-correct the avg_colour
        return Vec3(sqrt(avg_col.x), sqrt(avg_col.y), sqrt(avg_col.z));
    } else {
        return avg_col;
    }
}