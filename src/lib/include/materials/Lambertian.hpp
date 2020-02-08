/* LAMBERTIAN.hpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 7:08:28 PM
 * Last edited:
 *   2/8/2020, 1:43:47 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Lambertian class describes a mat material, that scatters light in
 *   a random direction. The colour of the material (i.e., what light it
 *   absorbs) can be specified upon creating the material. This particular
 *   file is the header file for Lambertian.cpp.
**/

#ifndef LAMBERTIAN_HPP
#define LAMBERTIAN_HPP

#include "../json.hpp"

#include "../Material.hpp"

namespace RayTracer {
    class Lambertian: public Material {
        public:
            Lambertian(const Vec3& colour_absorption);

            /* Computes how a ray reflects from or travels through the surface of the material. */
            virtual bool scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const;

            /* Returns a json object describing this Lambertian object. */
            virtual nlohmann::json to_json() const;
            /* Returns a fresh Lambertian object as described in given json object. */
            static Lambertian* from_json(nlohmann::json json_obj);

            Vec3 albedo;
    };
}

#endif