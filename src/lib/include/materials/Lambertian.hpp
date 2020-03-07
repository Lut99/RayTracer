/* LAMBERTIAN.hpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 7:08:28 PM
 * Last edited:
 *   07/03/2020, 11:48:29
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

#include "json.hpp"

#include "Material.hpp"

namespace RayTracer {
    class Lambertian: public Material {
        public:
            Lambertian(const Vec3& colour_absorption);
            /* Virtual deconstructor for Material to allow children to handle this if they need to. */
            virtual ~Lambertian() = default;

            /* Clone constructor for the Lambertian class. Basically a copy constructor, except that it allocates it (so it needs deallocating) and that the return type is Material*. */
            virtual Material* clone() const;

            /* Computes how a ray reflects from or travels through the surface of the material. */
            virtual bool scatter(const Ray&, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const;

            /* Returns a json object describing this Lambertian object. */
            virtual nlohmann::json to_json() const;
            /* Returns a fresh Lambertian object as described in given json object. */
            static Lambertian* from_json(nlohmann::json json_obj);

            Vec3 albedo;
    };
}

#endif