/* MATERIAL.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 10:19:23 PM
 * Last edited:
 *   2/9/2020, 1:47:24 AM
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
        protected:
            /* The Material class is purely virtual, and should only be constructor from any child classes. */
            Material(MaterialType material_type);

            /* This function compiles Material-general properies to a given json object. */
            virtual void baseclass_to_json(nlohmann::json& json_obj) const;
        public:
            const MaterialType type;

            /* Virtual deconstructor for Material to allow children to handle this if they need to. */
            virtual ~Material() = default;

            /* This function is meant to be implemented in the child classes of Material to allow for polymorphic copying. */
            virtual Material* clone() const = 0;

            /* Computes how a ray reflects from or travels through the surface of the material. */
            virtual bool scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const = 0;

            /* Virtual for the to_json function of the Material's children */
            virtual nlohmann::json to_json() const = 0;
            /* Static function that gets a derived material class from given json object. Note that the returned value is allocated and will have to be deallocated. */
            static Material* from_json(nlohmann::json json_obj);
    };
}

#endif