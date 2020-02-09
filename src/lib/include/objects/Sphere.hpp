/* SPHERE.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:37:11 PM
 * Last edited:
 *   2/9/2020, 1:44:27 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Sphere class represents a 3D sphere in the space. Additionally, it
 *   also features some useful mathmatical functions and the hit function
 *   for the Ray. Is derived from the Shape class. This particular file is
 *   the header file for Sphere.cpp.
**/

#ifndef SPHERE_HPP
#define SPHERE_HPP

#include "../json.hpp"

#include "../RenderObject.hpp"
#include "../Material.hpp"

namespace RayTracer {
    class Sphere: public RenderObject {
        public:
            double radius;
            Material* material;

            /* The Sphere class computes how a ray interacts with a Sphere object. The Sphere is defined by an origin (vector) and a radius. */
            Sphere(const Vec3& origin, double radius, Material* material);
            /* Copy constructor for the Sphere class. */
            Sphere(const Sphere& other);
            /* Move constructor for the Sphere class. */
            Sphere(Sphere&& other);
            virtual ~Sphere();

            /* The clone method returns a reference to a new RenderObject method. */
            virtual RenderObject* clone() const;

            /* Computes whether given Ray hits this Sphere. If it doesn't, return a t of -1. If it does, returns the length of the Ray until it hits the Sphere. Not that t_min and t_max are exclusive. */
            virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const;

            /* Returns the colour at the hitpoint of the Sphere. */
            virtual Vec3 colour(const HitRecord& record) const;

            /* Returns the normal of the sphere at given point. */
            virtual Vec3 normal(const HitRecord& record) const;

            /* Returns a json object representing this Sphere object */
            virtual nlohmann::json to_json() const;
            /* Creates a newly allocated Sphere object from given json object. Note that the new object will have to be deallocated manually. */
            static Sphere* from_json(nlohmann::json json_obj);
    };
}

#endif