/* DIELECTRIC.hpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 7:15:28 PM
 * Last edited:
 *   1/29/2020, 7:16:01 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Dielectric material describes a material that light can pass
 *   through. Not only that, it passes through while refracting, and
 *   sometimes even reflects it (like glass or water). This particular file
 *   is the header file for Dielectric.cpp.
**/

#ifndef DIELECTRIC_HPP
#define DIELECTRIC_HPP

#include "../Material.hpp"

namespace RayTracer {
    class Dielectric: public Material {
        public:
            Dielectric(const Vec3& colour_absorption, double ri);

            /* Computes how a ray reflects from or travels through the surface of the material. */
            virtual bool scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const;
            /* Computes the reflection of given ray over given normal. */
            virtual Vec3 reflect(const Vec3& v, const Vec3& n) const;
            /* Computes how a ray refracts when breaking through or out the surface of this RenderObject. */
            virtual bool refract(const Vec3& v, const Vec3& n, double ni_over_nt, Vec3& refracted) const;
            /* Does some magic. */
            virtual double schlick(double cosine) const;

            Vec3 albedo;
            double ref_idx;
    };
}

#endif