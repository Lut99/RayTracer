/* RAY.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 12:51:47 PM
 * Last edited:
 *   1/22/2020, 12:56:32 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Ray class represent a ray in the 3D space. This particular file is
 *   the implementation for Ray.hpp.
**/

#include "include/Ray.hpp"

using namespace std;
using namespace RayTracer;

Ray::Ray() {
    this->origin = Vec3();
    this->direction = Vec3();
}
Ray::Ray(const Vec3& origin, const Vec3& direction) {
    this->origin = origin;
    this->direction = direction;
}

Vec3 Ray::point(double t) {
    return this->origin + t * this->direction;
}