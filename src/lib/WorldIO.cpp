/* WORLD IO.cpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 7:41:40 PM
 * Last edited:
 *   2/8/2020, 11:06:35 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The WorldIO class can save and load RenderWorld objects to a JSON
 *   file. Note that it defines separate readers and writers for objects,
 *   materials and some other classes, so that they can be re-used if
 *   needed. This particular file is the implementation file for
 *   WorldIO.hpp.
**/

#include <iostream>

#include "include/scenes/RandomScene.hpp"
#include "include/WorldIO.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;

/* RENDERWORLD OBJECTS */
json WorldIO::to_json(const RenderWorld& world) {
    json j;
    j["objects"] = json::array();
    j["lights"] = json::array();

    for (size_t i = 0; i < world.get_object_count(); i++) {
        j["objects"][i] = WorldIO::Objects::to_json(world.get_object(i));
    }
    // TODO: Light

    return j;
}
RenderWorld* WorldIO::from_json(const json& json_obj) {
    // Check if the object has an object type
    if (!json_obj.is_object()) {
        throw InvalidTypeException("RenderWorld", json::object().type_name(), json_obj.type_name());
    }

    // Check if the required field exists and is an array
    if (json_obj["objects"].is_null()) {
        throw MissingFieldException("RenderWorld", "objects");
    }
    if (json_obj["lights"].is_null()) {
        throw MissingFieldException("RenderWorld", "lights");
    }
    if (json_obj["objects"] == "random") {
        // Special case: return a randomly generated world
        return new RandomScene();
    } else if (!json_obj["objects"].is_array()) {
        throw InvalidFieldFormat("RenderWorld", "objects", json::array().type_name(), json_obj["objects"].type_name());
    }
    if (!json_obj["lights"].is_array()) {
        throw InvalidFieldFormat("RenderWorld", "lights", json::array().type_name(), json_obj["lights"].type_name());
    }

    // Parse all the items in this array
    RenderWorld* world = new RenderWorld();
    for (std::size_t i = 0; i < json_obj["objects"].size(); i++) {
        world->add_object(WorldIO::Objects::from_json(json_obj["objects"][i]));
    }
    // TODO: Lights

    return world;
}

