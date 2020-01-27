/* CAMERA.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 3:23:14 PM
 * Last edited:
 *   1/27/2020, 3:00:32 PM
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


Camera::Camera(Vec3 lookfrom, Vec3 lookat, Vec3 up, double vfov, int screen_width, int screen_height, int rays_per_pixel, bool correct_gamma)
    : width(screen_width),
    height(screen_height),
    rays(rays_per_pixel),
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

Ray Camera::get_ray(int x, int y) const {
    // Compute u and v
    double u, v;
    if (this->rays > 1) {
        u = double(x + random_double()) / double(this->width);
        v = double(this->height - 1 - y + random_double()) / double(this->height);
    } else {
        u = double(x) / double(this->width);
        v = double(this->height - 1 - y) / double(this->height);
    }

    // Return a ray shot through this pixel
    return Ray(this->origin, this->lower_left_corner + u * this->horizontal + v * this->vertical - this->origin);
}
