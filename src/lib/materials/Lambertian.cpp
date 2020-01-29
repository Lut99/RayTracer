/* LAMBERTIAN.cpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 7:08:10 PM
 * Last edited:
 *   1/29/2020, 7:13:23 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Lambertian class describes a mat material, that scatters light in
 *   a random direction. The colour of the material (i.e., what light it
 *   absorbs) can be specified upon creating the material. This particular
 *   file is the implementation file for Lambertian.hpp.
**/

#include "../include/Random.hpp"
#include "../include/materials/Lambertian.hpp"

using namespace std;
using namespace RayTracer;


Lambertian::Lambertian(const Vec3& colour_absorption)
    : Material(lambertian),
    albedo(colour_absorption)
{}

// NOTE: For self, attenuation is like the decay of light in the medium.
bool Lambertian::scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const {
    Vec3 target = record.hitpoint + record.normal + random_in_unit_sphere();
    ray_out = Ray(record.hitpoint, target - record.hitpoint);
    attenuation = this->albedo;
    return true;
}