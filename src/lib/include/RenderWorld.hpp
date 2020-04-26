/* RENDER WORLD.hpp
 *   by Lut99
 *
 * Created:
 *   1/27/2020, 2:30:56 PM
 * Last edited:
 *   2/9/2020, 2:02:59 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The RenderWorld class is representing the 3D world. This is where the
 *   computation happens like shooting rays and bouncing them, while the
 *   Camera handles only the viewport-related stuff. An additional
 *   advantage of the RenderWorld is the easier use with respect to the
 *   ThreadPool class. This particular file described the headers for
 *   RenderWorld.cpp.
**/

#ifndef RENDERWORLD_HPP
#define RENDERWORLD_HPP

#include <limits>
#include <vector>
#include <chrono>

#include "json.hpp"
#include "Vec3.hpp"
#include "RenderObject.hpp"
#include "ObjectTree.hpp"
#include "Camera.hpp"
#include "RenderAnimation.hpp"

namespace RayTracer {
    class RenderWorld {
        private:
            /* Optimisable list that stores the RenderObjects in the world. */
            ObjectTree* objects;
        public:
            /* The RenderWorld class describes the world, and serves as the root for the object hiearchy. */
            RenderWorld();
            /* Copy constructor for the RenderWorld class. */
            RenderWorld(const RenderWorld& other);
            /* Move constructor for the RenderWorld class. */
            RenderWorld(RenderWorld&& other);
            virtual ~RenderWorld();

            /* Adds a RenderObject to the world. Note that anything added this way will be deallocated automatically. */
            void add_object(RenderObject* obj);

            /* Returns the number of objects defined in the RenderWorld. */
            std::size_t get_object_count() const;

            /* Computes the colour of a shot ray. The depth variable makes sure that, in the case of very reflective surfaces, we only do this at max 50 times. */
            Vec3 bounce_ray(const Ray& ray, int depth=0) const;
            /* Renders one pixel instead by shooting rays and everything. The optional cam_index can be used to render from another camera if there are multiple given. The index is equal to the order the cameras were added. */
            Vec3 render_pixel(int x, int y, const Camera& cam) const;

            /* Updates all animations in the RenderWorld */
            void update(std::chrono::milliseconds time_passed);

            /* Wraps the internal optimize function of the ObjectTree. */
            void optimize();

            /* Copy assignment operator for the RenderWorld class. */
            RenderWorld& operator=(RenderWorld other);
            /* Move assignment operator for the RenderWorld class. */
            RenderWorld& operator=(RenderWorld&& other);

            /* Swap operator for the RenderWorld class. */
            friend void swap(RenderWorld& first, RenderWorld& second);

            /* Returns a json object representing this RenderWorld and all RenderObjects, RenderLights and RenderAnimations stored in it. */
            virtual nlohmann::json to_json() const;
            /* Returns a new RenderWorld based on the given json file. Note that this new world will have to be deallocated later on. */
            static RenderWorld* from_json(nlohmann::json json_obj);
    };

    /* Swap operator for the RenderWorld class. */
    void swap(RenderWorld& first, RenderWorld& second);
}

#endif