/* RENDER WORLD.cpp
 *   by Lut99
 *
 * Created:
 *   1/27/2020, 2:30:39 PM
 * Last edited:
 *   2/9/2020, 4:09:21 PM
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

#include "include/scenes/RandomScene.hpp"
#include "include/JSONExceptions.hpp"
#include "include/RenderWorld.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


RenderWorld::RenderWorld() {
    // Nothing much, really
}
RenderWorld::RenderWorld(const RenderWorld& other) {
    // Deepcopy the vectors

    // Deepcopy the objects
    for (size_t i = 0; i < other.objects.size(); i++) {
        this->objects.push_back(other.objects.at(i)->clone());
    }

    // Deepcopy the lights
    // for (size_t i = 0; i < other.lights.size(); i++) {
    //     this->lights.push_back(other.lights.at(i)->clone());
    // }
}
RenderWorld::RenderWorld(RenderWorld&& other) {
    // Simply shallow copy the vectors
    this->objects = other.objects;
    this->lights = other.lights;

    // Now fill the object's vectors with empty ones to avoid deallocation of the objects
    other.objects = vector<RenderObject*>();
    other.lights = vector<int*>();
}

RenderWorld::~RenderWorld() {
    // Undeclare everything in the vectors
    for (std::size_t i = 0; i < this->objects.size(); i++) {
        delete this->objects.at(i);
    }
    // for (std::size_t i = 0; i < this->lights.size(); i++) {
    //     delete this->lights.at(i);
    // }
}



void RenderWorld::add_object(RenderObject* obj) {
    this->objects.push_back(obj);
}

void RenderWorld::add_light(int* light) {
    
}



RenderObject& RenderWorld::get_object(unsigned int obj_index) const {
    // Check if not out of bounds
    if (obj_index >= this->objects.size()) {
        throw out_of_range("Object index " + to_string(obj_index) + " is out of range for World with " + to_string(this->objects.size()) + " objects.");
    }

    // Return the object
    return *this->objects.at(obj_index);
}
int& RenderWorld::get_light(unsigned int light_index) const {
    // Check if not out of bounds
    if (light_index >= this->lights.size()) {
        throw out_of_range("Light index " + to_string(light_index) + " is out of range for World with " + to_string(this->lights.size()) + " lights.");
    }

    // Return the object
    return *this->lights.at(light_index);
}

std::size_t RenderWorld::get_object_count() const {
    return this->objects.size();
}
std::size_t RenderWorld::get_light_count() const {
    return this->lights.size();
}




Vec3 RenderWorld::bounce_ray(const Ray& ray, int depth) const {
    // Find the hit with the smallest hit
    HitRecord record;
    double t_best = numeric_limits<double>::max();
    bool hit = false;
    for (std::size_t i = 0; i < this->objects.size(); i++) {
        if (this->objects.at(i)->hit(ray, 0.0, t_best, record)) {
            // Store this hit as the best one
            t_best = record.t;
            hit = true;
        }
    }

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
        return Vec3(sqrt(avg_col.x), sqrt(avg_col.y), sqrt(avg_col.z));
    } else {
        return avg_col;
    }
}

void RenderWorld::update(chrono::milliseconds time_passed) {
    // Loop through all objects and update them
    for (size_t i = 0; i < this->objects.size(); i++) {
        this->objects[i]->update(time_passed);
    }
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
    this->lights = other.lights;

    // Now fill the object's vectors with empty ones to avoid deallocation of the objects
    other.objects = vector<RenderObject*>();
    other.lights = vector<int*>();

    // Return a reference to this
    return *this;
}

void RayTracer::swap(RenderWorld& first, RenderWorld& second) {
    using std::swap;

    swap(first.objects, second.objects);
    swap(first.lights, second.lights);
}



json RenderWorld::to_json() const {
    json j;
    j["objects"] = json::array();
    j["lights"] = json::array();

    for (size_t i = 0; i < this->objects.size(); i++) {
        j["objects"][i] = this->objects[i]->to_json();
    }

    // for (size_t i = 0; i < this->lights.size(); i++) {
    //     j["lights"][i] = this->lights[i].to_json();
    // }

    return j;
}
RenderWorld* RenderWorld::from_json(nlohmann::json json_obj) {
    // SPECIAL CASE: If the json_obj is simply 'random', return the special random scene
    if (json_obj == "random") {
        // Generate random objects
        return new RandomScene();
    }

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
    
    // Check if the fields are arrays
    if (!json_obj["objects"].is_array()) {
        throw InvalidFieldFormat("RenderWorld", "objects", json::array().type_name(), json_obj["objects"].type_name());
    }
    if (!json_obj["lights"].is_array()) {
        throw InvalidFieldFormat("RenderWorld", "lights", json::array().type_name(), json_obj["lights"].type_name());
    }

    // Parse all the items in this array
    RenderWorld* world = new RenderWorld();

    // Parse the objects
    for (std::size_t i = 0; i < json_obj["objects"].size(); i++) {
        world->add_object(RenderObject::from_json(json_obj["objects"][i]));
    }
    
    // Parse the lights
    /*
    for (std::size_t i = 0; i < json_obj["lights"].size(); i++) {
        world->add_light(RenderLight::from_json(json_obj["lights"][i]));
    }
    **/

    return world;
}