/* RENDER OBJECT COLLECTION.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 2:26:08 PM
 * Last edited:
 *   2/9/2020, 1:43:47 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The RenderObjectCollection class is derived from the RenderObject, and
 *   can contain multiple RenderObjects. It serves as a way to handle
 *   multiple objects elegantly as one. This particular file is the
 *   implementation file for RenderObjectCollection.hpp.
**/

#include <stdexcept>

#include "../include/JSONExceptions.hpp"
#include "../include/objects/RenderObjectCollection.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


Vec3 RenderObjectCollection::compute_center(vector<RenderObject*> objects) const {
    Vec3 average;
    for (std::size_t i = 0; i < objects.size(); i++) {
        average += objects[i]->center;
    }
    return average / objects.size();
}

RenderObjectCollection::RenderObjectCollection(vector<RenderObject*> objects)
    : RenderObject(this->compute_center(objects), render_object_collection)
{
    // Store this vector by copying
    this->objects = objects;
}

RenderObject* RenderObjectCollection::clone() const {
    return (RenderObject*) new RenderObjectCollection(*this);
}



bool RenderObjectCollection::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const {
    HitRecord temp_record;
    double t_best = t_max;
    bool hit = false;
    // Loop until we found the hit with the smallest t. By overriding t_max, we can easily and elegantly do this.
    for (std::size_t i = 0; i < this->objects.size(); i++) {
        if (this->objects[i]->hit(ray, t_min, t_best, temp_record)) {
            hit = true;
            t_best = temp_record.t;
            record = temp_record;
        }
    }
    return hit;
}

Vec3 RenderObjectCollection::colour(const HitRecord& record) const {
    // Return the colour of the hit object
    return record.obj->colour(record);
}

Vec3 RenderObjectCollection::normal(const HitRecord& record) const {
    // Return the normal of the hit object
    return record.obj->normal(record);
}

size_t RenderObjectCollection::size() const {
    return this->objects.size();
}
RenderObject* RenderObjectCollection::get_object(size_t index) const {
    if (index >= this->size()) {
        throw out_of_range("Index " + to_string(index) + " is out of range for RenderObjectCollection with size " + to_string(this->size()));
    }

    return this->objects.at(index);
}

json RenderObjectCollection::to_json() const {
    json j;

    // First, let the parent class parse all it wants
    this->baseclass_to_json(j);
    
    // Add child-specific properties
    j["objects"] = json::array();
    // Append all the internal objects
    for (std::size_t i = 0; i < this->objects.size(); i++) {
        j["objects"][i] = this->objects[i]->to_json();
    }
    return j;
}
RenderObjectCollection* RenderObjectCollection::from_json(json json_obj) {
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
        objects.push_back(RenderObject::from_json(json_obj["objects"][i]));
    }

    return new RenderObjectCollection(objects);
}