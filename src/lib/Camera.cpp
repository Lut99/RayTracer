/* CAMERA.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 3:23:14 PM
 * Last edited:
 *   1/22/2020, 6:23:44 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Camera class acts as the viewport, and therefore holds information
 *   about the viewport's location and rotation. Additionally, it handles
 *   the anti-aliasing. This particular file is the implementation file for
 *   Camera.hpp.
**/

#include "include/ProgressBar.hpp"
#include "include/Random.hpp"
#include "include/Camera.hpp"

using namespace std;
using namespace RayTracer;


Camera::Camera(int screen_width, int screen_height, int rays_per_pixel, bool show_progressbar, bool correct_gamma)
    : width(screen_width),
    height(screen_height),
    rays(rays_per_pixel),
    progress(show_progressbar),
    gamma(correct_gamma)
{
    double ratio = (double) this->width / (double) this->height;

    this->lower_left_corner = Vec3(-2.0, -1.0, -1.0);
    this->horizontal = Vec3(4.0, 0.0, 0.0);
    this->vertical = Vec3(0.0, 4.0 / ratio, 0.0);
    this->origin = Vec3(0.0, 0.0, 0.0);
}

Ray Camera::get_ray(double u, double v) const {
    return Ray(this->origin, this->lower_left_corner + u * this->horizontal + v * this->vertical - this->origin);
}

/* Shoots a ray. If it hits something, apply diffusion. Otherwise, return the sky colour. */
Vec3 shoot_ray(const Ray& ray, const RenderObjectCollection& world) {
    HitRecord record;
    if (world.hit(ray, 0.0, numeric_limits<double>::max(), record)) {
        Vec3 target = record.hitpoint + record.hitpoint.normalize() + random_in_unit_sphere();
        return 0.5 * shoot_ray(Ray(record.hitpoint, (target - record.hitpoint).normalize()), world);
    } else {
        Vec3 unit = ray.direction.normalize();
        double t = 0.5 * (unit.y + 1.0);
        return (1.0 - t) * Vec3(1, 1, 1) + t * Vec3(0.5, 0.7, 1.0);
    }
}

Image Camera::render(const RenderObjectCollection& world) {
    Image out(this->width, this->height);
    ProgressBar prgrs(0, this->width * this->height - 1);
    for (int y = this->height-1; y >= 0; y--) {
        for (int x = 0; x < this->width; x++) {
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
                col += shoot_ray(ray, world);
            }

            // Compute the colour average
            Vec3 avg_col = col / this->rays;
            if (this->gamma) {
                // Gamma-correct the avg_colour
                out[y][x] = sqrt(avg_col);
            } else {
                out[y][x] = avg_col;
            }

            // Update the progress bar if needed
            if (this->progress) {
                prgrs.update();
            }
        }
    }
    return out;
}