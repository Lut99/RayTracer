/* RENDER WORLD.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 4:10:32 PM
 * Last edited:
 *   1/22/2020, 4:42:14 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The RenderWorld class is a modified RenderObjectCollection class (and
 *   inherits from it). This class is meant to be the root of the object
 *   tree, and is the only class that will actively diffure (since it has
 *   access to all other objects). This particular file is the
 *   implementation for RenderWorld.hpp.
**/

#include "include/Random.hpp"
#include "include/RenderWorld.hpp"

using namespace std;
using namespace RayTracer;

RenderWorld::RenderWorld(Camera& cam, vector<RenderObject*> objects)
    :RenderObjectCollection(objects)
{
    this->camera = &cam;
}

Vec3 RenderWorld::colour(const HitRecord& record) const {
    Vec3 new_target = record.hitpoint + record.hitpoint.normalize() + random_in_unit_sphere();
    // Bounce a new ray
    Ray new_ray = Ray(record.hitpoint, new_target - record.hitpoint);
    HitRecord new_record;
    if (this->hit(new_ray, record.t_min, record.t_max, new_record)) {
        // Bounced; recursive call
        return 0.5 * this->colour(new_record);
    } else {
        // Not bounced; return the default value
        return this->camera->get_default_background(new_ray);
    }
}
