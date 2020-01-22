/* SHAPE.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:38:50 PM
 * Last edited:
 *   1/22/2020, 2:18:45 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Shape class provides the basis for all basic shapes that can be
 *   rendered. Is meant to be virtual, so only use from the shapes
 *   themselves such as a sphere. This file is the implementation for
 *   Shape.hpp.
**/

#include <stdexcept>

#include "include/RenderObject.hpp"

using namespace std;
using namespace RayTracer;

RenderObject::RenderObject(const Vec3& origin, const RenderObjectType type)
    :type(type)
{
    this->origin = origin;
}

bool RenderObject::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const {
    throw runtime_error("Function RenderObject::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) is not overridden.");
}

Vec3 RenderObject::colour(const Vec3& hitpoint) const {
    throw runtime_error("Function RenderObject::colour(const Vec3& hitpoint) is not overridden.");
}

Vec3 RenderObject::normal(const Vec3& point) const {
    throw runtime_error("Function RenderObject::normal(const Vec3& point) is not overridden.");
}