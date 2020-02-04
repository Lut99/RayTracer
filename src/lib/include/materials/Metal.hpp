/* METAL.hpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 7:12:18 PM
 * Last edited:
 *   2/4/2020, 4:29:22 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Metal class describes a material that reflects, i.e. a material
 *   that reflects light off in a constant direction. The colour and blur
 *   of the metal can be specified at creation of the metal. This
 *   particular file is the header file for Metal.cpp.
**/

#ifndef METAL_HPP
#define METAL_HPP

#include "../json.hpp"
#include "../Material.hpp"

namespace RayTracer {
    class Metal: public Material {
        public:
            Metal(const Vec3& colour_absorption, double fuzziness);

            /* Computes how a ray reflects from or travels through the surface of the material. */
            virtual bool scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const;
            /* Computes the reflection of given ray over given normal. */
            virtual Vec3 reflect(const Vec3& v, const Vec3& n) const;

            /* Returns a json object describing this Metal object. */
            virtual nlohmann::json to_json() const;
            /* Returns a fresh Metal object as described in given json object. */
            static Metal from_json(nlohmann::json json_obj);

            Vec3 albedo;
            double fuzz;
    };
}

#endif