/* SHAPE.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:38:50 PM
 * Last edited:
 *   2/8/2020, 11:01:56 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Shape class provides the basis for all basic shapes that can be
 *   rendered. Is meant to be virtual, so only use from the shapes
 *   themselves such as a sphere. This file is the implementation for
 *   Shape.hpp.
**/

#include <stdexcept>

#include "include/objects/Sphere.hpp"
#include "include/objects/RenderObjectCollection.hpp"

#include "include/JSONExceptions.hpp"
#include "include/RenderObject.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


RenderObject::RenderObject(const Vec3& center, const RenderObjectType type)
    : type(type)
{
    this->center = center;
}

bool RenderObject::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const {
    throw runtime_error("Function RenderObject::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) is not overridden.");
}

Vec3 RenderObject::colour(const HitRecord& record) const {
    throw runtime_error("Function RenderObject::colour(const HitRecord& record) is not overridden.");
}

Vec3 RenderObject::normal(const HitRecord& record) const {
    throw runtime_error("Function RenderObject::normal(const HitRecord& record) is not overridden.");
}

json RenderObject::to_json() const {
    throw runtime_error("Function RenderObject::to_json() is not overridden.");
}
RenderObject* RenderObject::from_json(json json_obj) {
    // Check if the object has an object type
    if (!json_obj.is_object()) {
        throw InvalidTypeException("RenderObject", json::object().type_name(), json_obj.type_name());
    }

    // Check for the type field
    if (json_obj["type"].is_null()) {
        throw MissingFieldException("RenderObject", "type");
    }

    // Parse the raw type
    RenderObjectType type;
    try {
        type = (RenderObjectType) json_obj["type"].get<unsigned long>();
    } catch (nlohmann::detail::type_error& e) {
        throw InvalidFieldFormat("RenderObject", "type", "unsigned long", json_obj["type"].type_name());
    }

    // Return the correct parser
    if (type == sphere) {
        return Sphere::from_json(json_obj);
    } else if (type == render_object_collection) {
        return RenderObjectCollection::from_json(json_obj);
    } else {
        throw UnknownSubtypeException("RenderObject", type);
    }
}