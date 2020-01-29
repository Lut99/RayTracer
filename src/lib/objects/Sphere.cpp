/* SPHERE.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:36:39 PM
 * Last edited:
 *   1/29/2020, 7:20:27 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Sphere class represents a 3D sphere in the space. Additionally, it
 *   also features some useful mathmatical functions and the hit function
 *   for the Ray. Is derived from the Shape class. This particular file is
 *   the implementation for Sphere.hpp.
**/

#include <iostream>
#include <math.h>
#include <stdexcept>

#include "../include/objects/Sphere.hpp"

using namespace std;
using namespace RayTracer;

Sphere::Sphere(const Vec3& origin, double radius, Material* material)
    : RenderObject(origin, sphere)
{
    this->radius = radius;
    this->material = material;
}
Sphere::~Sphere() {
    delete this->material;
}

bool Sphere::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const {
    Vec3 d_ray_sphere = ray.origin - this->center;
    double a = dot(ray.direction, ray.direction);
    double b = dot(d_ray_sphere, ray.direction);
    double c = dot(d_ray_sphere, d_ray_sphere) - this->radius * this->radius;

    double D = b*b - a*c;

    // Return the distance the ray has travelled until it hits the closest side of the sphere.
    if (D > 0) {
        // Compute tboth variations of the t. If t - sqrt(D) is true, take that one. Otherwise, take the other.
        double t = (-b - sqrt(D)) / a;
        if (t <= t_min || t >= t_max) {
            t = (-b + sqrt(D)) / a;
            if (t <= t_min || t >= t_max) {
                return false;
            }
        }

        record.t = t;
        record.t_min = t_min;
        record.t_max = t_max;
        record.hitpoint = ray.point(t);
        record.normal = this->normal(record);
        record.obj = (RenderObject*) this;
        record.material = this->material;

        return true;
    }
    return false;
}

Vec3 Sphere::colour(const HitRecord& record) const {
    return 0.5 * (Vec3(record.normal.x, record.normal.y, record.normal.z) + 1);
}

Vec3 Sphere::normal(const HitRecord& record) const {
    return (record.hitpoint - this->center) / this->radius;
}