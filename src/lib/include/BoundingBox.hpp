/* BOUNDING BOX.hpp
 *   by Lut99
 *
 * Created:
 *   3/20/2020, 12:00:05 PM
 * Last edited:
 *   3/20/2020, 12:43:47 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The BoundingBox class is a simple wrapper for the BoundingBoxes used
 *   in the optimised tree search for objects. It does not much more than
 *   to store the two points that make up the box and one function to merge
 *   it with a larger one, and one function to check if it has been hit by
 *   a ray. This file is the header file for BoundingBox.cpp.
**/

#ifndef BOUNDINGBOX_HPP
#define BOUNDINGBOX_HPP

#include "Ray.hpp"
#include "Vec3.hpp"

namespace RayTracer {
    class BoundingBox {
        public:
            Vec3 p1;
            Vec3 p2;

            /* The BoundingBox class contains the two points of a bounding box and some useful functions. By default, everything is set to (0,0,0). */
            BoundingBox(): p1(0, 0, 0), p2(0, 0, 0) {}
            /* The BoundingBox class contains the two points of a bounding box and some useful functions. This overload inits it with vectors. */
            BoundingBox(Vec3 p1, Vec3 p2): p1(p1), p2(p2) {}

            /* Checks whether this boundingbox is hit by given ray or not. */
            bool hit(const Ray& ray, double t_min, double t_max) const;

            /* Enlarges the inner BoundingBox until it encompasses given box. */
            void merge(const BoundingBox& other);

            /* Returns the volume of the bounding box. */
            double size() const;
    };
}

#endif