/* MATERIAL.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 10:19:23 PM
 * Last edited:
 *   2/8/2020, 10:49:51 PM
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

#include "json.hpp"

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

            /* Virtual for the to_json function of the Material's children */
            virtual nlohmann::json to_json() const;
            /* Static function that gets a derived material class from given json object. Note that the returned value is allocated and will have to be deallocated. */
            static Material* from_json(nlohmann::json json_obj);
    };
}

#endif