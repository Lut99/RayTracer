/* SPHERE.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:36:39 PM
 * Last edited:
 *   2/9/2020, 1:46:16 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Sphere class represents a 3D sphere in the space. Additionally, it
 *   also features some useful mathmatical functions and the hit function
 *   for the Ray. Is derived from the Shape class. This particular file is
 *   the implementation for Sphere.hpp.
**/

#include <iostream>
#include <math.h>
#include <stdexcept>

#include "../include/JSONExceptions.hpp"
#include "../include/objects/Sphere.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


Sphere::Sphere(const Vec3& center, double radius, Material* material)
    : RenderObject(center, sphere)
{
    this->radius = radius;
    this->material = material;
}

Sphere::Sphere(const Sphere& other)
    : RenderObject(other.center, sphere)
{
    this->radius = other.radius;
    this->material = other.material->clone();
}

Sphere::Sphere(Sphere&& other)
    : RenderObject(other.center, sphere)
{
    this->radius = other.radius;
    this->material = other.material;

    other.material = nullptr;
}

Sphere::~Sphere() {
    delete this->material;
}

RenderObject* Sphere::clone() const {
    return (RenderObject*) new Sphere(*this);
}



bool Sphere::hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const {
    Vec3 d_ray_sphere = ray.origin - this->center;
    double a = dot(ray.direction, ray.direction);
    double b = dot(d_ray_sphere, ray.direction);
    double c = dot(d_ray_sphere, d_ray_sphere) - this->radius * this->radius;

    double D = b*b - a*c;

    // Return the distance the ray has travelled until it hits the closest side of the sphere.
    if (D > 0) {
        // Compute tboth variations of the t. If t - sqrt(D) is true, take that one. Otherwise, take the other.
        double t = (-b - sqrt(D)) / a;
        if (t <= t_min || t >= t_max) {
            t = (-b + sqrt(D)) / a;
            if (t <= t_min || t >= t_max) {
                return false;
            }
        }

        record.t = t;
        record.t_min = t_min;
        record.t_max = t_max;
        record.hitpoint = ray.point(t);
        record.normal = this->normal(record);
        record.obj = (RenderObject*) this;
        record.material = this->material;

        return true;
    }
    return false;
}

Vec3 Sphere::colour(const HitRecord& record) const {
    return 0.5 * (Vec3(record.normal.x, record.normal.y, record.normal.z) + 1);
}

Vec3 Sphere::normal(const HitRecord& record) const {
    return (record.hitpoint - this->center) / this->radius;
}



json Sphere::to_json() const {
    json j;

    // First, let the parent class parse all it wants
    this->baseclass_to_json(j);
    
    // Add child-specific properties
    j["center"] = this->center.to_json();
    j["radius"] = this->radius;
    j["material"] = this->material->to_json();
    return j;
}
Sphere* Sphere::from_json(json json_obj) {
    // Check if the object has an object type
    if (!json_obj.is_object()) {
        throw InvalidTypeException("Sphere", json::object().type_name(), json_obj.type_name());
    }

    // Check for the required fields
    if (json_obj["center"].is_null()) {
        throw MissingFieldException("Sphere", "center");
    }
    if (json_obj["radius"].is_null()) {
        throw MissingFieldException("Sphere", "radius");
    }
    if (json_obj["material"].is_null()) {
        throw MissingFieldException("Sphere", "material");
    }

    // Parse them
    Vec3 center = Vec3::from_json(json_obj["center"]);
    double radius;
    try {
        radius = json_obj["radius"].get<double>();
    } catch (nlohmann::detail::type_error& e) {
        throw InvalidFieldFormat("Sphere", "radius", "double", json_obj["radius"].type_name());
    }
    Material* mat = Material::from_json(json_obj["material"]);

    return new Sphere(center, radius, mat);
}