/* CAMERA MOVEMENT.cpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 4:48:11 PM
 * Last edited:
 *   2/9/2020, 10:20:27 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The CameraMovement class describes how the Camera can move around. It
 *   is in itself another animation baseclass, as there are multiple types
 *   of camera movements. This particular file is the implementation for
 *   CameraMovement.hpp.
**/

#include <stdexcept>

#include "CameraRotation.hpp"

#include "JSONExceptions.hpp"
#include "CameraMovement.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


CameraMovement::CameraMovement(CameraMovementType movement_type)
    : type(movement_type)
{}



void CameraMovement::baseclass_to_json(json& json_obj) const {
    json_obj["type"] = (unsigned long) this->type;
}

json CameraMovement::to_json() const {
    throw runtime_error("Function CameraMovement::to_json() is not overridden.");
}

CameraMovement* CameraMovement::from_json(json json_obj) {
    // Check if the object has an object type
    if (!json_obj.is_object()) {
        throw InvalidTypeException("CameraMovement", json::object().type_name(), json_obj.type_name());
    }

    // Check if the required type field exists and is an array
    if (json_obj["type"].is_null()) {
        throw MissingFieldException("CameraMovement", "type");
    }
    
    // Parse the raw type
    CameraMovementType cam_type;
    try {
        cam_type = (CameraMovementType) json_obj["type"].get<unsigned long>();
    } catch (nlohmann::detail::type_error&) {
        throw InvalidFieldFormat("CameraMovement", "type", "unsigned long", json_obj["type"].type_name());
    }

    // Decide how to parse the rest of the JSON
    if (cam_type == rotation) {
        return (CameraMovement*) CameraRotation::from_json(json_obj);
    } else {
        throw UnknownSubtypeException("CameraMovement", cam_type);
    }
}