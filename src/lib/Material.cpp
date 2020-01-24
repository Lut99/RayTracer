/* MATERIAL.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 10:18:56 PM
 * Last edited:
 *   1/24/2020, 11:05:59 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Material class described how an incoming ray reflects from the
 *   surface. This way, objects can have different light-working
 *   properties. This particular file implements the Material.hpp file.
**/

#include <stdexcept>

#include "include/Random.hpp"
#include "include/Material.hpp"

using namespace std;
using namespace RayTracer;

bool Material::scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const {
    throw runtime_error("Function Material::scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) is not overridden.");
}



/* MATERIAL TYPES */
Lambertian::Lambertian(const Vec3& colour_absorption)
    :albedo(colour_absorption)
{}

// NOTE: For self, attenuation is like the decay of light in the medium.
bool Lambertian::scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const {
    Vec3 target = record.hitpoint + record.normal + random_in_unit_sphere();
    ray_out = Ray(record.hitpoint, target - record.hitpoint);
    attenuation = this->albedo;
    return true;
}



Metal::Metal(const Vec3& colour_absorption, double fuzziness)
    :albedo(colour_absorption),
    fuzz(fuzziness)
{}

Vec3 Metal::reflect(const Vec3& v, const Vec3& n) const {
    return v - 2 * dot(n, v) * n;
}
bool Metal::scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const {
    Vec3 reflected = this->reflect(ray_in.direction.normalize(), record.normal);
    ray_out = Ray(record.hitpoint, reflected + this->fuzz*random_in_unit_sphere());
    attenuation = this->albedo;
    return (dot(ray_out.direction, record.normal) > 0);
}



Dielectric::Dielectric(const Vec3& colour_absorption, double refrective_index)
    : albedo(colour_absorption),
    ref_idx(refrective_index)
{}

bool Dielectric::scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const {
    Vec3 outward_normal;
    double ni_over_nt;
    attenuation = this->albedo;
    Vec3 refracted;

    double reflect_prob;
    double cosine;

    // I think this decides on what side of the sphere we hit; in to out or out to in.
    if (dot(ray_in.direction, record.normal) > 0) {
        outward_normal = -record.normal;
        ni_over_nt = this->ref_idx;
        cosine = this->ref_idx * dot(ray_in.direction, record.normal) / ray_in.direction.length();
    } else {
        outward_normal = record.normal;
        ni_over_nt = 1.0 / this->ref_idx;
        cosine = -dot(ray_in.direction, record.normal) / ray_in.direction.length();
    }

    if (this->refract(ray_in.direction, outward_normal, ni_over_nt, refracted)) {
        reflect_prob = this->schlick(cosine);
    } else {
        reflect_prob = 1.0;
    }

    if (random_double() < reflect_prob) {
        ray_out = Ray(record.hitpoint, this->reflect(ray_in.direction, record.normal));
    } else {
        ray_out = Ray(record.hitpoint, refracted);
    }

    return true;
}
Vec3 Dielectric::reflect(const Vec3& v, const Vec3& n) const {
    return v - 2 * dot(n, v) * n;
}
bool Dielectric::refract(const Vec3& v, const Vec3& n, double ni_over_nt, Vec3& refracted) const {
    Vec3 v_normalized = v.normalize();
    double dt = dot(v_normalized, n);
    double D = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (D > 0) {
        refracted = ni_over_nt * (v_normalized - n * dt) - n * sqrt(D);
        return true;
    }
    return false;
}
double Dielectric::schlick(double cosine) const {
    double r0 = (1 - this->ref_idx) / (1 + this->ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}