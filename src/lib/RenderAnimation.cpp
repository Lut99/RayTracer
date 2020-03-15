/* RENDER ANIMATION.cpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 2:15:15 PM
 * Last edited:
 *   2/9/2020, 2:00:23 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The RenderAnimation class defines a movement or change of texture on an
 *   object. They can be separately added to the RenderWorld with the
 *   target RenderObject in mind, and will change properties that are
 *   available to the RenderObject. Note that some animations, such as
 *   texture change, cannot be added for each object. Also, the special
 *   CameraAnimation child class can mutate the camera mid-render. This
 *   particular file is the implementation file for RenderAnimation.hpp.
**/

#include <stdexcept>

#include "CameraMovement.hpp"

#include "JSONExceptions.hpp"
#include "RenderAnimation.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


RenderAnimation::RenderAnimation(RenderAnimationType animation_type)
    : type(animation_type)
{}



void RenderAnimation::baseclass_to_json(json& json_obj) const {
    json_obj["type"] = (unsigned long) this->type;
}



json RenderAnimation::to_json() const {
    throw runtime_error("Function RenderAnimation::to_json() is not overridden.");
}
RenderAnimation* RenderAnimation::from_json(json json_obj) {
    // Check if the object has an object type
    if (!json_obj.is_object()) {
        throw InvalidTypeException("RenderAnimation", json::object().type_name(), json_obj.type_name());
    }

    // Check if the required type field exists and is an array
    if (json_obj["type"].is_null()) {
        throw MissingFieldException("RenderAnimation", "type");
    }
    
    // Parse the raw type
    RenderAnimationType type;
    try {
        type = (RenderAnimationType) json_obj["type"].get<unsigned long>();
    } catch (nlohmann::detail::type_error&) {
        throw InvalidFieldFormat("RenderAnimation", "type", "unsigned long", json_obj["type"].type_name());
    }

    // Decide how to parse the rest of the JSON
    if (true) {
        throw UnknownSubtypeException("RenderAnimation", type);
    }
}