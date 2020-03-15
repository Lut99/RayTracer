/* SHAPE.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:39:23 PM
 * Last edited:
 *   2/9/2020, 1:50:49 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Shape class provides the basis for all basic shapes that can be
 *   rendered. Is meant to be virtual, so only use from the shapes
 *   themselves such as a sphere. This file is the header file for
 *   Shape.cpp.
**/

#ifndef SHAPE_HPP
#define SHAPE_HPP

#include <string>
#include <chrono>

#include "json.hpp"
#include "Vec3.hpp"
#include "Ray.hpp"
#include "HitRecord.hpp"
#include "Material.hpp"
#include "RenderAnimation.hpp"

namespace RayTracer {
    class RenderAnimation;

    enum RenderObjectType {
        sphere,
        render_object_collection
    };
    static std::string RenderObjectTypeNames[] {
        "sphere",
        "render_object_collection"
    };

    class RenderObject {
        protected:
            RenderAnimation* animation;

            /* This function compiles RenderObject-general properies to a given json object. */
            virtual void baseclass_to_json(nlohmann::json& json_obj) const;
        public:
            const RenderObjectType type;
            Vec3 center;

            Vec3 hit_1;
            Vec3 hit_2;
            bool has_hitbox;

            /* Default deconstructor for the RenderObject class, except that it's virtual so that baseclasses can improve upon this. */
            virtual ~RenderObject() = default;
            /* The clone method returns a reference to a new RenderObject method. This is purely virtual, and should be implemented by the derived classes as if it were a copy constructor. */
            virtual RenderObject* clone() const = 0;

            /* The RenderObject class is virtual, and should not be used by yourself. Only derived classes should call this function. */
            RenderObject(const Vec3& center, const RenderObjectType type);

            /* A quick check if the bounding box of the object has been hit by a ray. The bounding box limits should be computed at the object's init. Note that the normal hit function is needed for more accuracy. */
            virtual bool quick_hit(const Ray& ray, double t_min, double t_max);
            /* Virtual for the derived classes to recompute the hit boxes. */
            virtual bool compute_hit_box() = 0;

            /* Virtual for the derived classes to determine whether they have been hit by a ray or not. */
            virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const = 0;

            /* Virtual for the derived classes to return the colour associated with the hit position. */
            virtual Vec3 colour(const HitRecord& record) const = 0;

            /* Virtual for the normal of the derived class. */
            virtual Vec3 normal(const HitRecord& record) const = 0;

            /* Allows an animation to be specified for this particular RenderObject. Any overloads of this function might be able to handle animations specific to that type themselves, but then should call this version for the rest (if there are any left). */
            virtual void set_animation(RenderAnimation* animation);
            /* Updates this object based on given animation. Only needs to be overloaded if derived classes handle this in particular ways and for particular animation types. */
            virtual void update(std::chrono::milliseconds time_passed);

            /* Virtual for the derived classes to implement a custom to_json function. */
            virtual nlohmann::json to_json() const = 0;
            /* Static function that gets a derived material class from given json object. Note that the returned value is allocated and will have to be deallocated. */
            static RenderObject* from_json(nlohmann::json json_obj);
    };
}

#endif