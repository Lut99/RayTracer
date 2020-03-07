/* METAL.cpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 7:11:51 PM
 * Last edited:
 *   07/03/2020, 11:50:16
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Metal class describes a material that reflects, i.e. a material
 *   that reflects light off in a constant direction. The colour and blur
 *   of the metal can be specified at creation of the metal. This
 *   particular file is the implementation file for Metal.hpp.
**/

#include "Random.hpp"
#include "JSONExceptions.hpp"
#include "Metal.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


Metal::Metal(const Vec3& colour_absorption, double fuzziness)
    : Material(metal),
    albedo(colour_absorption),
    fuzz(fuzziness)
{}

Material* Metal::clone() const {
    return (Material*) new Metal(*this);
}



Vec3 Metal::reflect(const Vec3& v, const Vec3& n) const {
    return v - 2 * dot(n, v) * n;
}
bool Metal::scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const {
    Vec3 reflected = this->reflect(ray_in.direction.normalize(), record.normal);
    ray_out = Ray(record.hitpoint, reflected + this->fuzz*random_in_unit_sphere());
    attenuation = this->albedo;
    return (dot(ray_out.direction, record.normal) > 0);
}



json Metal::to_json() const {
    json j;

    // First, let the Material class compile
    this->baseclass_to_json(j);

    // Add our own properties
    j["albedo"] = this->albedo.to_json();
    j["fuzz"] = this->fuzz;
    return j;
}
Metal* Metal::from_json(json json_obj) {
    // Check if the object has an object type
    if (!json_obj.is_object()) {
        throw InvalidTypeException("Metal", json::object().type_name(), json_obj.type_name());
    }

    // Check if the required fields are there
    if (json_obj["albedo"].is_null()) {
        throw MissingFieldException("Metal", "albedo");
    }
    if (json_obj["fuzz"].is_null()) {
        throw MissingFieldException("Metal", "fuzz");
    }

    // Parse it
    Vec3 albedo = Vec3::from_json(json_obj["albedo"]);
    double fuzz;
    try {
        fuzz = json_obj["fuzz"].get<double>();
    } catch (nlohmann::detail::type_error& e) {
        throw InvalidFieldFormat("Metal", "fuzz", "double", json_obj["fuzz"].type_name());
    }

    return new Metal(albedo, fuzz);
}