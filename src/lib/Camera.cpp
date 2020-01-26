/* CAMERA.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 3:23:14 PM
 * Last edited:
 *   1/26/2020, 6:18:39 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Camera class acts as the viewport, and therefore holds information
 *   about the viewport's location and rotation. Additionally, it handles
 *   the anti-aliasing. This particular file is the implementation file for
 *   Camera.hpp.
**/

#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#include "include/ProgressBar.hpp"
#include "include/Random.hpp"
#include "include/Camera.hpp"

using namespace std;
using namespace RayTracer;


Camera::Camera(Vec3 lookfrom, Vec3 lookat, Vec3 up, double vfov, int screen_width, int screen_height, int rays_per_pixel, bool show_progressbar, bool correct_gamma)
    : width(screen_width),
    height(screen_height),
    rays(rays_per_pixel),
    progress(show_progressbar),
    gamma(correct_gamma)
{
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

    #ifdef CAMERA_THREADS
    // Also define the threadpool
    this->pool = new ThreadPool(500, *this);
    #endif
}
#ifdef CAMERA_THREADS
Camera::~Camera() {
    delete this->pool;
}
#endif

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
    ProgressBar prgrs(0, this->width * this->height - 1);

    auto start = chrono::system_clock::now();
    
    #ifndef CAMERA_THREADS

    for (int y = this->height-1; y >= 0; y--) {
        for (int x = 0; x < this->width; x++) {
            // Render the pixel
            out[y][x] = this->render_pixel(x, y, world);

            if (this->progress) {
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

    cout << "Rendered frame in " << ((double) duration_ms.count()) / 1000.0 << " seconds" << endl;

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