/* HIT RECORD.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 2:17:23 PM
 * Last edited:
 *   1/22/2020, 3:09:00 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file describes the HitRecord struct, which is nothing more than a
 *   bunch of variables collected after having computed whether a ray hits
 *   an object.
**/

#ifndef HITRECORD_HPP
#define HITRECORD_HPP

#include "RenderObject.hpp"
#include "Vec3.hpp"

namespace RayTracer {
    class RenderObject;

    struct HitRecord {
        double t;
        Vec3 hitpoint;
        RenderObject* obj;
    };
}

#endif