/* RENDER OBJECT COLLECTION.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 2:26:30 PM
 * Last edited:
 *   07/03/2020, 11:48:51
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The RenderObjectCollection class is derived from the RenderObject, and
 *   can contain multiple RenderObjects. It serves as a way to handle
 *   multiple objects elegantly as one. This particular file is the header
 *   file for RenderObjectCollection.cpp.
**/

#ifndef RENDEROBJECTCOLLECTION_HPP
#define RENDEROBJECTCOLLECTION_HPP

#include <vector>

#include "json.hpp"
#include "RenderObject.hpp"

namespace RayTracer {
    class RenderObjectCollection: public RenderObject {
        private:
            std::vector<RenderObject*> objects;

            /* Computers the center of the RenderObjectCollection by averaging all centers */
            Vec3 compute_center(std::vector<RenderObject*> objects) const;
        public:
            /* The RenderObjectCollection can contain multiple RenderObjects and treat those as if they were one. */
            RenderObjectCollection(std::vector<RenderObject*> objects);
            virtual ~RenderObjectCollection() = default;

            /* The clone method returns a reference to a new RenderObject method. */
            virtual RenderObject* clone() const;

            /* Returns the closest hit object from the internal collection of objects. */
            virtual bool hit(const Ray& ray, double t_min, double t_max, HitRecord& record) const;

            /* Returns the colour of any of the objects indicated by the hitpoint. */
            virtual Vec3 colour(const HitRecord& record) const;

            /* Returns the normal vector of the object at the indicated point. */
            virtual Vec3 normal(const HitRecord& record) const;

            /* Returns the number of objects in the collection. */
            std::size_t size() const;
            /* Returns a reference to an object at given index in the vector. */
            RenderObject* get_object(std::size_t index) const;

            /* Returns a json object representing this RenderObjectCollection object */
            virtual nlohmann::json to_json() const;
            /* Creates a newly allocated RenderObjectCollection object from given json object. Note that the new object will have to be deallocated manually. */
            static RenderObjectCollection* from_json(nlohmann::json json_obj);
    };
}

#endif