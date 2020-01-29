/* CAMERA.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 3:23:14 PM
 * Last edited:
 *   1/29/2020, 11:47:37 AM
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


Camera::Camera(Vec3 lookfrom, Vec3 lookat, Vec3 up, double vfov, double aperture, double focus_dist, int screen_width, int screen_height, int rays_per_pixel, bool correct_gamma)
    : width(screen_width),
    height(screen_height),
    rays(rays_per_pixel),
    gamma(correct_gamma)
{
    double ratio = (double) this->width / (double) this->height;
    this->lens_radius = aperture / 2;

    double theta = vfov * M_PI / 180;
    double half_height = tan(theta/2);
    double half_width = ratio * half_height;

    this->origin = lookfrom;
    this->w = (lookfrom - lookat).normalize();
    this->u = cross(up, w).normalize();
    this->v = cross(w, u);

    this->lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist *  v - focus_dist * w;
    this->horizontal = 2 * half_width * focus_dist * u;
    this->vertical = 2 * half_height * focus_dist * v;
}

Ray Camera::get_ray(int x, int y) const {
    // Compute u and v
    double s, t;
    if (this->rays > 1) {
        s = double(x + random_double()) / double(this->width);
        t = double(this->height - 1 - y + random_double()) / double(this->height);
    } else {
        s = double(x) / double(this->width);
        t = double(this->height - 1 - y) / double(this->height);
    }

    // Return a ray shot through this pixel
    Vec3 rd = this->lens_radius * random_in_unit_disc();
    Vec3 offset = this->u * rd.x + this->v * rd.y;
    return Ray(this->origin + offset, this->lower_left_corner + s * this->horizontal + t * this->vertical - this->origin - offset);
}
