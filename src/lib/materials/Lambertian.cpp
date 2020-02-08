/* LAMBERTIAN.cpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 7:08:10 PM
 * Last edited:
 *   2/9/2020, 12:20:53 AM
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
#include "../include/JSONExceptions.hpp"
#include "../include/materials/Lambertian.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


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

nlohmann::json Lambertian::to_json() const {
    json j;
    
    // First, let the Material class compile
    this->baseclass_to_json(j);

    // Add our own properties
    j["albedo"] = this->albedo.to_json();
    return j;
}

Lambertian* Lambertian::from_json(nlohmann::json json_obj) {
    // Check if the object has an object type
    if (!json_obj.is_object()) {
        throw InvalidTypeException("Lambertian", json::object().type_name(), json_obj.type_name());
    }

    // Check if there is an albedo field
    if (json_obj["albedo"].is_null()) {
        throw MissingFieldException("Lambertian", "albedo");
    }

    // Parse it with the Vec3 parser
    Vec3 albedo = Vec3::from_json(json_obj["albedo"]);

    return new Lambertian(albedo);
}