/* RENDER OBJECT COLLECTION.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 2:26:08 PM
 * Last edited:
 *   07/03/2020, 11:51:10
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

#include "JSONExceptions.hpp"
#include "RenderObjectCollection.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


/* Takes four vectors for bounding boxes and enlarges the first Vec's to encompass the second one. */
void surroundBox(Vec3& box_A1, Vec3& box_A2, const Vec3& box_B1, const Vec3& box_B2) {
    // For each of the three dimensions, find the smallest and put that in
    //   box_A1. The larger is put in box_A2.
    for (int i = 0; i < 3; i++) {
        double a1 = box_A1.get(i);
        double a2 = box_A2.get(i);
        double b1 = box_B1.get(i);
        double b2 = box_B2.get(i);

        // Order the A-box values
        double min = a1;
        double max = a2;
        if (min > max) {
            min = max;
            max = a1;
        }

        // Add the B-box values
        if (b1 < min) {
            min = b1;
        } else if (b1 > max) {
            max = b1;
        }
        if (b2 < min) {
            min = b2;
        } else if (b2 > max) {
            max = b2;
        }

        // Set the values and return
        box_A1[i] = min;
        box_A2[i] = max;
    }
}


Vec3 RenderObjectCollection::compute_center(vector<RenderObject*> objects) const {
    Vec3 average;
    for (std::size_t i = 0; i < objects.size(); i++) {
        average += objects[i]->center;
    }
    return average / (double) objects.size();
}

RenderObjectCollection::RenderObjectCollection(vector<RenderObject*> objects)
    : RenderObject(this->compute_center(objects), render_object_collection)
{
    // Store this vector by copying
    this->objects = vector<RenderObject*>(objects);

    // Be sure to compute the hitbox
    this->has_hitbox = this->compute_hit_box();
}
RenderObjectCollection::RenderObjectCollection(const RenderObjectCollection& other)
    : RenderObject(other.center, render_object_collection)
{
    // Copy the vector of the other
    this->objects = vector<RenderObject*>(other.objects);

    // Copy the hitbox vectors if needed
    this->has_hitbox = other.has_hitbox;
    if (this->has_hitbox) {
        this->hit_1 = Vec3(other.hit_1);
        this->hit_2 = Vec3(other.hit_2);
    }
}
RenderObjectCollection::RenderObjectCollection(RenderObjectCollection&& other)
    : RenderObject(other.center, render_object_collection)
{
    // Steal the vector
    this->objects = std::move(other.objects);
    other.objects = vector<RenderObject*>();

    // Also steal the vectors (if needed)
    this->has_hitbox = other.has_hitbox;
    if (this->has_hitbox) {
        this->hit_1 = other.hit_1;
        this->hit_2 = other.hit_2;
    }
}

RenderObject* RenderObjectCollection::clone() const {
    return (RenderObject*) new RenderObjectCollection(*this);
}



bool RenderObjectCollection::compute_hit_box() {
    // If there are no elements, there is no box
    size_t s = this->size();
    if (s < 1) {
        return false;
    }

    // Otherwise, get the box of the first one if it has one
    if (!this->objects.at(0)->has_hitbox) {
        return false;
    }
    this->hit_1 = this->objects.at(0)->hit_1;
    this->hit_2 = this->objects.at(0)->hit_2;

    // Combine the boxes into an increasingly larger box
    for (size_t i = 1; i < s; i++) {
        if (!this->objects.at(i)->has_hitbox) {
            return false;
        }

        surroundBox(this->hit_1, this->hit_2, this->objects.at(i)->hit_1, this->objects.at(i)->hit_2);
    }

    return true;
}



bool RenderObjectCollection::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const {
    HitRecord temp_record;
    double t_best = t_max;
    bool hit = false;
    // Loop until we found the hit with the smallest t. By overriding t_max, we can easily and elegantly do this.
    for (std::size_t i = 0; i < this->objects.size(); i++) {
        if (this->objects[i]->quick_hit(ray, t_min, t_best) && this->objects[i]->hit(ray, t_min, t_best, temp_record)) {
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