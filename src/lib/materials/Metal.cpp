/* METAL.cpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 7:11:51 PM
 * Last edited:
 *   1/29/2020, 7:13:19 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Metal class describes a material that reflects, i.e. a material
 *   that reflects light off in a constant direction. The colour and blur
 *   of the metal can be specified at creation of the metal. This
 *   particular file is the implementation file for Metal.hpp.
**/

#include "../include/Random.hpp"
#include "../include/materials/Metal.hpp"

using namespace std;
using namespace RayTracer;


Metal::Metal(const Vec3& colour_absorption, double fuzziness)
    : Material(metal),
    albedo(colour_absorption),
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