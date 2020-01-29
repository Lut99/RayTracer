/* RENDER OBJECT COLLECTION.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 2:26:08 PM
 * Last edited:
 *   1/29/2020, 9:00:50 PM
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

#include "../include/objects/RenderObjectCollection.hpp"

using namespace std;
using namespace RayTracer;

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

const size_t RenderObjectCollection::size() const {
    return this->objects.size();
}
RenderObject* RenderObjectCollection::get_object(size_t index) const {
    if (index < 0 || index >= this->size()) {
        throw out_of_range("Index " + to_string(index) + " is out of range for RenderObjectCollection with size " + to_string(this->size()));
    }

    return this->objects.at(index);
}