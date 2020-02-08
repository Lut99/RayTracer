/* MATERIAL.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 10:18:56 PM
 * Last edited:
 *   2/9/2020, 12:20:12 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Material class described how an incoming ray reflects from the
 *   surface. This way, objects can have different light-working
 *   properties. This particular file implements the Material.hpp file.
**/

#include <stdexcept>

#include "include/materials/Lambertian.hpp"
#include "include/materials/Metal.hpp"
#include "include/materials/Dielectric.hpp"

#include "include/JSONExceptions.hpp"
#include "include/Random.hpp"
#include "include/Material.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


Material::Material(MaterialType material_type)
    : type(material_type)
{}




bool Material::scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) const {
    throw runtime_error("Function Material::scatter(const Ray& ray_in, const HitRecord& record, Vec3& attenuation, Ray& ray_out) is not overridden.");
}



void Material::baseclass_to_json(json& json_obj) const {
    json_obj["type"] = (unsigned long) this->type;
}

json Material::to_json() const {
    throw runtime_error("Function Material::to_json() is not overridden.");
}

Material* Material::from_json(json json_obj) {
    // Check if the object has an object type
    if (!json_obj.is_object()) {
        throw InvalidTypeException("Material", json::object().type_name(), json_obj.type_name());
    }

    // Check for the type field
    if (json_obj["type"].is_null()) {
        throw MissingFieldException("Material", "type");
    }

    // Parse the raw type
    MaterialType type;
    try {
        type = (MaterialType) json_obj["type"].get<unsigned long>();
    } catch (nlohmann::detail::type_error& e) {
        throw InvalidFieldFormat("Material", "type", "unsigned long", json_obj["type"].type_name());
    }

    // Return the correct parser
    if (type == lambertian) {
        return (Material*) Lambertian::from_json(json_obj);
    } else if (type == metal) {
        return (Material*) Metal::from_json(json_obj);
    } else if (type == dielectric) {
        return (Material*) Dielectric::from_json(json_obj);
    } else {
        throw UnknownSubtypeException("Material", type);
    }
}