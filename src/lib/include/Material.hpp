/* MATERIAL.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 10:19:23 PM
 * Last edited:
 *   1/29/2020, 7:19:22 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Material class described how an incoming ray reflects from the
 *   surface. This way, objects can have different light-working
 *   properties. This particular file contains the headers for
 *   Material.cpp.
**/

#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include <string>

#include "Ray.hpp"
#include "HitRecord.hpp"

namespace RayTracer {
    enum MaterialType {
        lambertian,
        metal,
        dielectric
    };
    static std::string MaterialTypeNames[] = {
        "lambertian",
        "metal",
        "dielectric"
    };


    class Material {
        public:
            const MaterialType type;
            Material(MaterialType material_type);

            /* Computes how a ray reflects from or travels through the surface of the material. */
            virtual bool scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const;
    };
}

#endif