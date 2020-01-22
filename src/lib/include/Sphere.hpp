/* SPHERE.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:37:11 PM
 * Last edited:
 *   1/22/2020, 2:03:04 PM
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

            /* The Sphere class computes how a ray interacts with a Sphere object. The Sphere is defined by an origin (vector) and a radius. */
            Sphere(const Vec3& origin, double radius);

            /* Computes whether given Ray hits this Sphere. If it doesn't, return a t of -1. If it does, returns the length of the Ray until it hits the Sphere. */
            double hit(const Ray& ray) const;

            /* Returns the colour at the hitpoint of the Sphere. */
            Vec3 colour(const Vec3& hitpoint) const;

            /* Returns the normal of the sphere at given point. */
            Vec3 normal(const Vec3& point) const;
    };
}

#endif