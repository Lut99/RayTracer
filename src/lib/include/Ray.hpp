/* RAY.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 12:52:07 PM
 * Last edited:
 *   1/22/2020, 12:55:59 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Ray class represent a ray in the 3D space. This particular file is
 *   the header file for Ray.cpp.
**/

#ifndef RAY_HPP
#define RAY_HPP

#include "Vec3.hpp"

namespace RayTracer {
    class Ray {
        public:
            Vec3 origin;
            Vec3 direction;

            /* The Ray class represents a light ray in the 3D space. The default initializer initializes the Ray with origin & direction (0,0,0). */
            Ray();
            /* The Ray class represents a light ray in the 3D space. This initializer initializes the Ray with given origin and direction. */
            Ray(const Vec3& origin, const Vec3& direction);

            /* Returns a point along the Ray with distance t from the origin. */
            Vec3 point(double t);
    };
}

#endif