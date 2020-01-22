/* SHAPE.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:38:50 PM
 * Last edited:
 *   1/22/2020, 1:44:48 PM
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

#include "include/Shape.hpp"

using namespace std;
using namespace RayTracer;

Shape::Shape(const Vec3& origin, const ShapeType type)
    :type(type)
{
    this->origin = origin;
}

double Shape::hit(const Ray& ray) const {
    throw runtime_error("Function Shape::hit(const Ray& ray) is not overridden.");
}

Vec3 Shape::colour(const Vec3& hitpoint) const {
    throw runtime_error("Function Shape::colour(const Vec3& hitpoint) is not overridden.");
}