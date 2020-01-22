/* RENDER OBJECT COLLECTION.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 2:26:08 PM
 * Last edited:
 *   1/22/2020, 2:54:34 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The RenderObjectCollection class is derived from the RenderObject, and
 *   can contain multiple RenderObjects. It serves as a way to handle
 *   multiple objects elegantly as one. This particular file is the
 *   implementation file for RenderObjectCollection.hpp.
**/

#include "include/RenderObjectCollection.hpp"

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
    this->n_objects = objects.size();
}

bool RenderObjectCollection::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const {
    HitRecord temp_record;
    double t_best = t_max;
    bool hit = false;
    // Loop until we found the hit with the smallest t. By overriding t_max, we can easily and elegantly do this.
    for (std::size_t i = 0; i < this->n_objects; i++) {
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