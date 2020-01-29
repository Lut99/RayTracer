/* MATERIAL.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 10:18:56 PM
 * Last edited:
 *   1/29/2020, 7:16:22 PM
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


Material::Material(MaterialType material_type)
    : type(material_type)
{}

bool Material::scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const {
    throw runtime_error("Function Material::scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) is not overridden.");
}