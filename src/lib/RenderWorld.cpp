/* RENDER WORLD.cpp
 *   by Lut99
 *
 * Created:
 *   1/27/2020, 2:30:39 PM
 * Last edited:
 *   3/3/2020, 4:42:27 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The RenderWorld class is representing the 3D world. This is where the
 *   computation happens like shooting rays and bouncing them, while the
 *   Camera handles only the viewport-related stuff. An additional
 *   advantage of the RenderWorld is the easier use with respect to the
 *   ThreadPool class. This particular file handles the implementation for
 *   RenderWorld.hpp.
**/

#include <iostream>
#include <stdexcept>
#include <cerrno>
#include <dlfcn.h>

#include "RandomScene.hpp"
#include "JSONExceptions.hpp"
#include "RenderWorld.hpp"

#if defined MACOS || defined _WIN32
#define sqrtf64 sqrt
#endif

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


RenderWorld::RenderWorld() {
    this->objects = new ObjectTree();
}
RenderWorld::RenderWorld(const RenderWorld& other) {
    this->objects = new ObjectTree(*other.objects);
}

RenderWorld::RenderWorld(RenderWorld&& other) {
    // Now fill the object's vectors with empty ones to avoid deallocation of the objects
    other.objects = other.objects;

    // Empty the reference in the other list
    other.objects = nullptr;
}

RenderWorld::~RenderWorld() {
    // Simply delete the list which will deallocate the objects
    delete this->objects;
}



void RenderWorld::add_object(RenderObject* obj) {
    this->objects->add(obj);
}



std::size_t RenderWorld::get_object_count() const {
    return this->objects->size();
}




Vec3 RenderWorld::bounce_ray(const Ray& ray, int depth) const {
    // Find the hit with the smallest hit
    HitRecord record;
    bool hit = this->objects->hit(ray, 0.0, numeric_limits<double>::max(), record);

    // Check if a hit occured
    if (hit) {
        Ray scattered;
        Vec3 attenuation;
        if (depth < 50 && record.material->scatter(ray, record, attenuation, scattered)) {
            return attenuation * this->bounce_ray(scattered, depth + 1);
        } else {
            return Vec3(0, 0, 0);
        }
    } else {
        Vec3 unit = ray.direction.normalize();
        double t = 0.5 * (unit.y + 1.0);
        return (1.0 - t) * Vec3(1, 1, 1) + t * Vec3(0.5, 0.7, 1.0);
    }
}

Vec3 RenderWorld::render_pixel(int x, int y, const Camera& cam) const {
    Vec3 col;
    for (int r = 0; r < cam.rays; r++) {
        Ray ray = cam.get_ray(x, y);

        // Get the colour
        col += this->bounce_ray(ray);
    }

    // Compute the colour average
    Vec3 avg_col = col / cam.rays;
    if (cam.gamma) {
        // Gamma-correct the avg_colour
        return Vec3(sqrtf64(avg_col.x), sqrtf64(avg_col.y), sqrtf64(avg_col.z));
    } else {
        return avg_col;
    }
}

void RenderWorld::update(chrono::milliseconds time_passed) {
    // Loop through all objects and update them
    for (size_t i = 0; i < this->objects->size(); i++) {
        this->objects->operator[](i)->update(time_passed);
    }
    // Re-optimise the tree
    this->objects->optimize();
}



void RenderWorld::optimize() {
    this->objects->optimize();
}



RenderWorld& RenderWorld::operator=(RenderWorld other) {
    // Only do some stuff if this != other
    if (this != &other) {
        // Swap this data with the other data
        swap(*this, other);
    }
    return *this;
}

RenderWorld& RenderWorld::operator=(RenderWorld&& other) {
    // Simply shallow copy the vectors
    this->objects = other.objects;

    // Now fill the object's vectors with empty ones to avoid deallocation of the objects
    other.objects = nullptr;

    // Return a reference to this
    return *this;
}

void RayTracer::swap(RenderWorld& first, RenderWorld& second) {
    using std::swap;

    swap(first.objects, second.objects);
}



json RenderWorld::to_json() const {
    json j;
    j["objects"] = json::array();

    for (size_t i = 0; i < this->objects->size(); i++) {
        j["objects"][i] = this->objects->operator[](i)->to_json();
    }

    return j;
}
RenderWorld* RenderWorld::from_json(nlohmann::json json_obj) {
    // Check if the object has an object type
    if (!json_obj.is_object()) {
        throw InvalidObjectFormat("RenderWorld", json::object().type_name(), json_obj.type_name());
    }

    // Parse all the items in this array
    RenderWorld* world = new RenderWorld();

    // Parse the objects field if it exists
    if (!json_obj["objects"].is_null()) {
        // Check if the fields are arrays
        if (!json_obj["objects"].is_array()) {
            delete world;
            throw InvalidFieldFormat("RenderWorld", "objects", json::array().type_name(), json_obj["objects"].type_name());
        }

        // Parse the objects
        for (std::size_t i = 0; i < json_obj["objects"].size(); i++) {
            world->add_object(RenderObject::from_json(json_obj["objects"][i]));
        }
    }
    
    // Parse the init function field
    if (!json_obj["init"].is_null()) {
        // Check if it is a string
        if (!json_obj["init"].is_string()) {
            delete world;
            throw InvalidFieldFormat("RenderWorld", "init", "string", json_obj["init"].type_name());
        }

        // Check if there is a file ending in .so with that name
        std::string path = json_obj["init"].get<std::string>();
        if (FILE* file = fopen(path.c_str(), "r")) {
            fclose(file);
        } else {
            // Doesn't exist
            delete world;
            throw FileIOException("RenderWorld", path, errno);
        }

        // There is, so try to load it using the dl library
        void* handle = dlopen(path.c_str(), RTLD_NOW);
        if (handle == NULL) {
            throw SOLoadException("RenderWorld", path, errno);
        }
        void (*init)(const RenderWorld& world) = (void (*)(const RenderWorld& world)) dlsym(handle, "init");
        if (init == NULL) {
            dlclose(handle);
            throw SOFuncLoadException("RenderWorld", path, "init", errno);
        }

        // Run the function on the given world
        try {
            (*init)(*world);
        } catch (std::exception& e) {
            dlclose(handle);
            throw SORunException("RenderWorld", path, "init", string(e.what()));
        }

        // Close the library
        dlclose(handle);
    }

    // Optimize the inner tree
    world->optimize();

    return world;
}