/* HIT RECORD.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 2:17:23 PM
 * Last edited:
 *   1/22/2020, 11:10:54 PM
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

namespace RayTracer {
    struct HitRecord;
}

#include "RenderObject.hpp"
#include "Material.hpp"
#include "Vec3.hpp"

namespace RayTracer {
    class RenderObject;
    class Material;

    struct HitRecord {
        double t;
        double t_min;
        double t_max;
        Vec3 hitpoint;
        Vec3 normal;
        RenderObject* obj;
        Material* material;
    };
}

#endif