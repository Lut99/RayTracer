/* SHAPE.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:39:23 PM
 * Last edited:
 *   1/29/2020, 7:20:02 PM
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

#include <string>

#include "Vec3.hpp"
#include "Ray.hpp"
#include "HitRecord.hpp"
#include "Material.hpp"

namespace RayTracer {
    enum RenderObjectType {
        sphere,
        render_object_collection
    };
    static std::string RenderObjectTypeNames[] {
        "sphere",
        "render_object_collection"
    };

    class RenderObject {
        public:
            const RenderObjectType type;
            Vec3 center;

            /* The RenderObject class is virtual, and should not be used by yourself. Only derived classes should call this function. */
            RenderObject(const Vec3& center, const RenderObjectType type);

            /* Virtual for the derived classes to determine whether they have been hit by a ray or not. */
            virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const;

            /* Virtual for the derived classes to return the colour associated with the hit position. */
            virtual Vec3 colour(const HitRecord& record) const;

            /* Virtual for the normal of the derived class. */
            virtual Vec3 normal(const HitRecord& record) const;
    };
}

#endif