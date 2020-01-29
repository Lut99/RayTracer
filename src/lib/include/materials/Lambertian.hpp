/* LAMBERTIAN.hpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 7:08:28 PM
 * Last edited:
 *   1/29/2020, 7:10:02 PM
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

#include "../Material.hpp"

namespace RayTracer {
    class Lambertian: public Material {
        public:
            Lambertian(const Vec3& colour_absorption);

            /* Computes how a ray reflects from or travels through the surface of the material. */
            virtual bool scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const;

            Vec3 albedo;
    };
}

#endif