/* SHAPE.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:39:23 PM
 * Last edited:
 *   1/22/2020, 1:47:07 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Shape class provides the basis for all basic shapes that can be
 *   rendered. Is meant to be virtual, so only use from the shapes
 *   themselves such as a sphere. This file is the header file for
 *   Shape.cpp.
**/

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "Vec3.hpp"
#include "Ray.hpp"

namespace RayTracer {
    enum ShapeType {
        sphere
    };

    class Shape {
        public:
            const ShapeType type;
            Vec3 origin;

            /* The Shape class is virtual, and should not be used by yourself. Only derived classes should call this function. */
            Shape(const Vec3& origin, const ShapeType type);

            /* Virtual for the derived classes to determine whether they have been hit by a ray or not. */
            virtual double hit(const Ray& ray) const;

            /* Virtual for the derived classes to return the colour associated with the hit position. */
            virtual Vec3 colour(const Vec3& hitpoint) const;
    };
}

#endif