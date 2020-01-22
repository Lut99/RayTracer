/* SHAPE.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:39:23 PM
 * Last edited:
 *   1/22/2020, 2:40:03 PM
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
#include "HitRecord.hpp"

namespace RayTracer {
    enum RenderObjectType {
        sphere
    };

    class RenderObject {
        public:
            const RenderObjectType type;
            Vec3 origin;

            /* The Shape class is virtual, and should not be used by yourself. Only derived classes should call this function. */
            RenderObject(const Vec3& origin, const RenderObjectType type);

            /* Virtual for the derived classes to determine whether they have been hit by a ray or not. */
            virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const;

            /* Virtual for the derived classes to return the colour associated with the hit position. */
            virtual Vec3 colour(const HitRecord& record) const;

            /* Virtual for the normal of the derived class. */
            virtual Vec3 normal(const Vec3& point) const;
    };
}

#endif