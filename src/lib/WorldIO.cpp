/* WORLD IO.cpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 7:41:40 PM
 * Last edited:
 *   2/4/2020, 4:34:11 PM
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

/* VEC3 OBJECTS */
json WorldIO::to_json(const Vec3& vec3) {
    json j = json::array();
    j[0] = vec3.x;
    j[1] = vec3.y;
    j[2] = vec3.z;
    return j;
}
Vec3 WorldIO::vec3_from_json(const json& json_obj) {
    // First, check if the json_obj is an array
    if (!json_obj.is_array()) {
        throw InvalidTypeException("Vec3", json::array().type_name(), json_obj.type_name());
    }

    // Next, check for the array length
    if (json_obj.size() != 3) {
        throw InvalidLengthException("Vec3", 3, json_obj.size());
    }

    // Then, try to parse it as three doubles
    Vec3 to_return;
    try {
        to_return.x = json_obj[0].get<double>();
        to_return.y = json_obj[1].get<double>();
        to_return.z = json_obj[2].get<double>();
    } catch (nlohmann::detail::type_error& e) {
        throw InvalidTypeException("Vec3", "double", json_obj[0].type_name());
    }
    return to_return;
}



/* RENDEROBJECTS */
json WorldIO::Objects::to_json(const RenderObject& object) {
    // Return based on the type
    if (object.type == sphere) {
        return to_json((Sphere&) object);
    } else if (object.type == render_object_collection) {
        return to_json((RenderObjectCollection&) object);
    }
    return json();
}
RenderObject* WorldIO::Objects::from_json(const json& json_obj) {
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
        return sphere_from_json(json_obj);
    } else if (type == render_object_collection) {
        return renderobjectcollection_from_json(json_obj);
    } else {
        throw UnknownSubtypeException("RenderObject", type);
    }
}

json WorldIO::Objects::to_json(const Sphere& object) {
    json j;
    j["type"] = (unsigned long) object.type;
    j["center"] = WorldIO::to_json(object.center);
    j["radius"] = object.radius;
    j["material"] = WorldIO::Materials::to_json(*object.material);
    return j;
}
Sphere* WorldIO::Objects::sphere_from_json(const json& json_obj) {
    // Check if the object has an object type
    if (!json_obj.is_object()) {
        throw InvalidTypeException("Sphere", json::object().type_name(), json_obj.type_name());
    }

    // Check for the required fields
    if (json_obj["center"].is_null()) {
        throw MissingFieldException("Sphere", "center");
    }
    if (json_obj["radius"].is_null()) {
        throw MissingFieldException("Sphere", "radius");
    }
    if (json_obj["material"].is_null()) {
        throw MissingFieldException("Sphere", "material");
    }

    // Parse them
    Vec3 center = WorldIO::vec3_from_json(json_obj["center"]);
    double radius;
    try {
        radius = json_obj["radius"].get<double>();
    } catch (nlohmann::detail::type_error& e) {
        throw InvalidFieldFormat("Sphere", "radius", "double", json_obj["radius"].type_name());
    }
    Material* mat = WorldIO::Materials::from_json(json_obj["material"]);

    return new Sphere(center, radius, mat);
}

json WorldIO::Objects::to_json(const RenderObjectCollection& object) {
    json j;
    j["type"] = (unsigned long) object.type;
    j["objects"] = json::array();
    // Append all the internal objects
    for (std::size_t i = 0; i < object.size(); i++) {
        j["objects"][i] = to_json(*object.get_object(i));
    }
    return j;
}
RenderObjectCollection* WorldIO::Objects::renderobjectcollection_from_json(const json& json_obj) {
    // Check if the object has an object type
    if (!json_obj.is_object()) {
        throw InvalidTypeException("RenderObjectCollection", json::object().type_name(), json_obj.type_name());
    }

    // Check if the required field exists and is an array
    if (json_obj["objects"].is_null()) {
        throw MissingFieldException("RenderObjectCollection", "objects");
    }
    if (!json_obj["objects"].is_array()) {
        throw InvalidFieldFormat("RenderObjectCollection", "objects", json::array().type_name(), json_obj["objects"].type_name());
    }

    // Parse all the items in this array
    vector<RenderObject*> objects;
    for (std::size_t i = 0; i < json_obj["objects"].size(); i++) {
        objects.push_back(from_json(json_obj["objects"][i]));
    }

    return new RenderObjectCollection(objects);
}




/* MATERIALS */
json WorldIO::Materials::to_json(const Material& object) {
    // Return based on the type
    if (object.type == lambertian) {
        return to_json((Lambertian&) object);
    } else if (object.type == metal) {
        return to_json((Metal&) object);
    } else if (object.type == dielectric) {
        return to_json((Dielectric&) object);
    }
    return json();
}
Material* WorldIO::Materials::from_json(const json& json_obj) {
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
        return (Material*) WorldIO::Materials::lambertian_from_json(json_obj);
    } else if (type == metal) {
        return (Material*) WorldIO::Materials::metal_from_json(json_obj);
    } else if (type == dielectric) {
        return (Material*) WorldIO::Materials::dielectric_from_json(json_obj);
    } else {
        throw UnknownSubtypeException("Material", type);
    }
}

