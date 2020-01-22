/* SPHERE.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:36:39 PM
 * Last edited:
 *   1/22/2020, 2:54:58 PM
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
    : RenderObject(origin, sphere)
{
    this->radius = radius;
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
        double t1 = (-b - sqrt(D)) / (2.0 * a);
        double t2 = (-b + sqrt(D)) / (2.0 * a);
        double t;
        if (t1 < t2) {t = t1;}
        else {t = t2;}
        
        if (t > t_min && t < t_max) {
            record.t = t;
            record.hitpoint = ray.point(t);
            record.obj = (RenderObject*) this;

            return true;
        }
    }
    return false;
}

Vec3 Sphere::colour(const HitRecord& record) const {
    Vec3 n = this->normal(record);
    return 0.5 * (Vec3(n.x, n.y, n.z) + 1);
}

Vec3 Sphere::normal(const HitRecord& record) const {
    return (record.hitpoint - this->center).normalize();
}