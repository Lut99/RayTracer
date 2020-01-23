/* MATERIAL.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 10:18:56 PM
 * Last edited:
 *   1/23/2020, 9:00:03 AM
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
    return v - 2 * dot(v, n) * n;
}
bool Metal::scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const {
    Vec3 reflected = this->reflect(ray_in.direction.normalize(), record.normal);
    ray_out = Ray(record.hitpoint, reflected + fuzz*random_in_unit_sphere());
    attenuation = this->albedo;
    return (dot(ray_out.direction, record.normal) > 0);
}
