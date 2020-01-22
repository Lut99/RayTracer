/* SPHERE.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:36:39 PM
 * Last edited:
 *   1/22/2020, 2:08:05 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Sphere class represents a 3D sphere in the space. Additionally, it
 *   also features some useful mathmatical functions and the hit function
 *   for the Ray. Is derived from the Shape class. This particular file is
 *   the implementation for Sphere.hpp.
**/

#include <math.h>

#include "include/Sphere.hpp"

using namespace std;
using namespace RayTracer;

Sphere::Sphere(const Vec3& origin, double radius)
    : Shape(origin, sphere)
{
    this->radius = radius;
}

double Sphere::hit(const Ray& ray) const {
    Vec3 d_ray_sphere = ray.origin - this->origin;
    double a = dot(ray.direction, ray.direction);
    double b = 2.0 * dot(d_ray_sphere, ray.direction);
    double c = dot(d_ray_sphere, d_ray_sphere) - this->radius * this->radius;

    double D = b*b - 4*a*c;

    // Return the distance the ray has travelled until it hits the closest side of the sphere.
    if (D < 0) {
        return -1.0;
    } else {
        double t1 = (-b - sqrt(D)) / (2.0 * a);
        double t2 = (-b + sqrt(D)) / (2.0 * a);

        if (t1 < t2) {
            return t1;
        } else {
            return t2;
        }
    }
}

Vec3 Sphere::colour(const Vec3& hitpoint) const {
    Vec3 n = this->normal(hitpoint);
    return 0.5 * (Vec3(n.x, n.y, n.z) + 1);
}

Vec3 Sphere::normal(const Vec3& point) const {
    return (point - this->origin).normalize();
}