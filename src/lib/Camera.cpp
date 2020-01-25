/* CAMERA.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 3:23:14 PM
 * Last edited:
 *   1/25/2020, 1:02:59 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Camera class acts as the viewport, and therefore holds information
 *   about the viewport's location and rotation. Additionally, it handles
 *   the anti-aliasing. This particular file is the implementation file for
 *   Camera.hpp.
**/

#ifdef CAMERA_THREADS
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
#ifdef CAMERA_THREADS

#define BATCH_SIZE 50

class PixelBatch {
    public:
        atomic<int> startx;
        atomic<int> starty;
        atomic<int> stopx;
        atomic<int> stopy;
        atomic<int> size;

        PixelBatch() {}
        PixelBatch(int x1, int y1, int x2, int y2) {
            this->startx.store(x1);
            this->starty.store(y1);
            this->stopx.store(x2);
            this->stopy.store(y2);
            this->size.store((abs(x1 - x2) + 1) * (abs(y1 - y2) + 1));
        }
        PixelBatch(const PixelBatch& other) {
            int x1,y1,x2,y2;
            other.load(x1, y1, x2, y2);
            this->store(x1, y1, x2, y2);
        }

        PixelBatch& store(int& x1, int& y1, int& x2, int& y2) {
            this->startx.store(x1);
            this->starty.store(y1);
            this->stopx.store(x2);
            this->stopy.store(y2);
            this->size.store((abs(x1 - x2) + 1) * (abs(y1 - y2) + 1));
            return *this;
        }

        const PixelBatch& load(int& x1, int& y1, int& x2, int& y2) const {
            x1 = this->startx.load();
            y1 = this->starty.load();
            x2 = this->stopx.load();
            y2 = this->stopy.load();
            return *this;
        }

        PixelBatch& operator=(const PixelBatch& other) {
            int x1,y1,x2,y2;
            other.load(x1, y1, x2, y2);
            this->store(x1, y1, x2, y2);
        }
};

struct ThreadData {
    int id;
    pthread_t tid;

    PixelBatch batch;

    atomic<bool> thread_done;
    atomic<bool> main_done;

    Image* out;
    const RenderObject* world;
    const Camera* camera;
};

void* render_thread(void* v_args) {
    ThreadData* args = (ThreadData*) v_args;
    const Camera* cam = args->camera;

    while (!args->main_done.load()) {
        int x1,y1,x2,y2;
        args->batch.load(x1, y1, x2, y2);

        for (int y = y2; y >= y1; y--) {
            for (int x = x1; x <= x2; x++) {
                args->out[0][y][x] = cam->render_pixel(x, y, *args->world);
            }
        }

        args->thread_done.store(true);

        // Wait until we can continue
        while (!args->main_done.load() && args->thread_done.load()) {}
    }

    return NULL;
}

PixelBatch create_batch(const int w, const int h, unsigned long& batch_index) {
    int x1,y1,x2,y2;

    x1 = batch_index % w;
    y1 = batch_index / w;

    batch_index += BATCH_SIZE - 1;
    // Bind batch_index to a maximum of (w - 1) * (h - 1)
    if (batch_index > w * h) {
        batch_index = w * h - 1;
    }

    x2 = batch_index % w;
    y2 = batch_index / w;

    batch_index++;

    return PixelBatch(x1, y1, x2, y2);
}

#endif

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

    auto start = chrono::system_clock::now();
    
    #ifndef CAMERA_THREADS

    ProgressBar prgrs(0, this->width * this->height - 1);
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

    // Prepare the structs for the threads
    unsigned long batch_index = 0;
    ThreadData threads[CAMERA_THREADS];
    for (int i = 0; i < CAMERA_THREADS; i++) {
        // Start with a batch
        threads[i].id = i;

        // Check if we should already stop
        if (batch_index > (this->width - 1) * (this->height - 1)) {
            threads[i].main_done.store(true);
        }

        // Create the first batch for this thread
        threads[i].batch = create_batch(this->width, this->height, batch_index);

        // Init the booleans
        threads[i].thread_done.store(false);
        threads[i].main_done.store(false);

        // Init some other variables
        threads[i].out = &out;
        threads[i].world = &world;
        threads[i].camera = this;
    }

    // Run the threads
    for (int i = 0; i < CAMERA_THREADS; i++) {
        // Only create them if they have to
        if (!threads[i].main_done.load()) {
            pthread_create(&threads[i].tid, NULL, render_thread, (void*) &threads[i]);
        }
    }

    // Wait for all of them to reap
    long all_done = 0;
    ProgressBar prgs(0, this->height * this->width);
    while (all_done < this->height * this->width) {
        // Loop to find a thread that is done
        for (int i = 0; i < CAMERA_THREADS; i++) {
            if (!threads[i].main_done.load() && threads[i].thread_done.load()) {
                // Copy the amount done
                all_done += threads[i].batch.size.load();

                // Check if we should already stop
                if (batch_index >= this->width * this->height) {
                    threads[i].main_done.store(true);
                    threads[i].thread_done.store(false);
                    continue;
                }

                // Create a new batch
                threads[i].batch = create_batch(this->width, this->height, batch_index);

                // Signal to continue
                threads[i].thread_done.store(false);
            }
        }

        // Update the progressbar
        if (this->progress) {
            prgs.set(all_done);
        }
    }

    // Reap 'em
    for (int i = 0; i < CAMERA_THREADS; i++) {
        pthread_join(threads[i].tid, NULL);
    }

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