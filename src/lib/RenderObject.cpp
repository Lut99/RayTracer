/* SHAPE.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:38:50 PM
 * Last edited:
 *   2/9/2020, 1:02:09 AM
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

#include "Sphere.hpp"
#include "RenderObjectCollection.hpp"

#include "JSONExceptions.hpp"
#include "RenderObject.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


RenderObject::RenderObject(const Vec3& center, const RenderObjectType type)
    : type(type)
{
    this->center = center;
    this->animation = nullptr;
}



void RenderObject::baseclass_to_json(nlohmann::json& json_obj) const {
    json_obj["type"] = (unsigned int) this->type;
    if (this->animation != nullptr) {
        json_obj["animation"] = this->animation->to_json();
    }
}



bool RenderObject::quick_hit(const Ray& ray, double t_min, double t_max) const {
    if (!this->has_hitbox) { return false; }

    return this->box.hit(ray, t_min, t_max);
}



void RenderObject::set_animation(RenderAnimation* animation) {
    this->animation = animation;
    // Let the animation recompute itself based on this RenderObject
    this->animation->recompute(this);
}
void RenderObject::update(std::chrono::milliseconds time_passed) {
    if (this->animation != nullptr) {
        // Let the animation handle this
        this->animation->update(time_passed, this);
    }
}



RenderObject* RenderObject::from_json(json json_obj) {
    // Check if the object has an object type
    if (!json_obj.is_object()) {
        throw InvalidObjectFormat("RenderObject", json::object().type_name(), json_obj.type_name());
    }

    // Check for the type field
    if (json_obj["type"].is_null()) {
        throw MissingFieldException("RenderObject", "type");
    }

    // Parse the raw type
    RenderObjectType type;
    try {
        type = (RenderObjectType) json_obj["type"].get<unsigned long>();
    } catch (nlohmann::detail::type_error&) {
        throw InvalidFieldFormat("RenderObject", "type", "unsigned long", json_obj["type"].type_name());
    }
    // If the animation field is defined, parse it as such
    RenderAnimation* animation = nullptr;
    if (!json_obj["animation"].is_null()) {
        animation = RenderAnimation::from_json(json_obj["animation"]);
    }

    // Parse the rest of the object based on the type field
    RenderObject* to_return;
    if (type == sphere) {
        to_return = (RenderObject*) Sphere::from_json(json_obj);
    } else if (type == render_object_collection) {
        to_return = (RenderObject*) RenderObjectCollection::from_json(json_obj);
    } else {
        throw UnknownSubtypeException("RenderObject", type);
    }

    // If needed, add the animation
    if (animation != nullptr) {
        to_return->set_animation(animation);
    }

    // Return
    return to_return;
}