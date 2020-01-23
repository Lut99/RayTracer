/* MATERIAL.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 10:19:23 PM
 * Last edited:
 *   1/23/2020, 8:58:37 AM
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

#include "Ray.hpp"
#include "HitRecord.hpp"

namespace RayTracer {
    enum MaterialType {
        lambertian,
        metal
    };

    class Material {
        public:
            virtual bool scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const;
    };

    class Lambertian: public Material {
        public:
            Lambertian(const Vec3& colour_absorption);

            virtual bool scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const;

            Vec3 albedo;
    };

    class Metal: public Material {
        public:
            Metal(const Vec3& colour_absorption, double fuzziness);

            virtual bool scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const;
            virtual Vec3 reflect(const Vec3& v, const Vec3& n) const;

            Vec3 albedo;
            double fuzz;
    };
}

#endif