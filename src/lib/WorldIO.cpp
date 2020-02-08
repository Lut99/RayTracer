/* WORLD IO.cpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 7:41:40 PM
 * Last edited:
 *   2/8/2020, 11:02:34 PM
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

/* CAMERA OBJECTS */
json WorldIO::to_json(const Camera& cam) {
    json j;
    j["lookat"] = to_json(cam.lookat);
    j["lookfrom"] = to_json(cam.lookfrom);
    j["up"] = to_json(cam.up);
    j["vfov"] = cam.vfov;
    j["aperture"] = cam.aperture;
    return j;
}
Camera* WorldIO::camera_from_json(const json& json_obj) {
    // Check if it is an object
    if (!json_obj.is_object()) {
        throw InvalidTypeException("Camera", json::object().type_name(), json_obj.type_name());
    }

    // Next, check if the required fields are present
    if (json_obj["lookat"].is_null()) {
        throw MissingFieldException("RenderWorld", "lookat");
    }
    if (json_obj["lookfrom"].is_null()) {
        throw MissingFieldException("RenderWorld", "lookfrom");
    }
    if (json_obj["up"].is_null()) {
        throw MissingFieldException("RenderWorld", "up");
    }
    if (json_obj["vfov"].is_null()) {
        throw MissingFieldException("RenderWorld", "vfov");
    }
    if (json_obj["aperture"].is_null()) {
        throw MissingFieldException("RenderWorld", "aperture");
    }

    // Parse the objects
    Vec3 lookat = vec3_from_json(json_obj["lookat"]);
    Vec3 lookfrom = vec3_from_json(json_obj["lookfrom"]);
    Vec3 up = vec3_from_json(json_obj["up"]);
    double vfov, aperture;
    try {
        vfov = json_obj["vfov"].get<double>();
        aperture = json_obj["aperture"].get<double>();
    } catch (nlohmann::detail::type_error& e) {
        throw InvalidFieldFormat("RenderWorld", "vfov or aperture", "double", json_obj[0].type_name());
    }
    
    // Return a new Camera object
    return new Camera(lookfrom, lookat, up, vfov, aperture);
}


