/* SPHERE.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:37:11 PM
 * Last edited:
 *   1/22/2020, 1:51:52 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Sphere class represents a 3D sphere in the space. Additionally, it
 *   also features some useful mathmatical functions and the hit function
 *   for the Ray. Is derived from the Shape class. This particular file is
 *   the header file for Sphere.cpp.
**/

#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "Shape.hpp"

namespace RayTracer {
    class Sphere: public Shape {
        public:
            double radius;

            Sphere(const Vec3& origin, double radius)
                : Shape(origin, sphere)
            {
                this->radius = radius;
            }

            double hit(const Ray& ray) const {
                Vec3 d_ray_sphere = ray.origin - this->origin;
                double a = dot(ray.direction, ray.direction);
                double b = 2.0 * dot(d_ray_sphere, ray.direction);
                double c = dot(d_ray_sphere, d_ray_sphere) - this->radius * this->radius;

                double D = b*b - 4*a*c;

                // For now, return if there is two hitpoints (e.g., ray travels through sphere)
                if (D > 0) {
                    return 1.0;
                } else {
                    return 0.0;
                }
            }

            Vec3 colour(const Vec3& hitpoint) const {
                return Vec3(1, 0, 0);
            }
    };
}

#endif