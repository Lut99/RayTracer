/* MATERIAL.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 10:18:56 PM
 * Last edited:
 *   2/9/2020, 1:47:31 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Material class described how an incoming ray reflects from the
 *   surface. This way, objects can have different light-working
 *   properties. This particular file implements the Material.hpp file.
**/

#include <stdexcept>

#include "Lambertian.hpp"
#include "Metal.hpp"
#include "Dielectric.hpp"

#include "JSONExceptions.hpp"
#include "Random.hpp"
#include "Material.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


Material::Material(MaterialType material_type)
    : type(material_type)
{}



void Material::baseclass_to_json(json& json_obj) const {
    json_obj["type"] = (unsigned long) this->type;
}

Material* Material::from_json(json json_obj) {
    // Check if the object has an object type
    if (!json_obj.is_object()) {
        throw InvalidObjectFormat("Material", json::object().type_name(), json_obj.type_name());
    }

    // Check for the type field
    if (json_obj["type"].is_null()) {
        throw MissingFieldException("Material", "type");
    }

    // Parse the raw type
    MaterialType type;
    try {
        type = (MaterialType) json_obj["type"].get<unsigned long>();
    } catch (nlohmann::detail::type_error&) {
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