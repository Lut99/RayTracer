/* RENDER WORLD.hpp
 *   by Lut99
 *
 * Created:
 *   1/27/2020, 2:30:56 PM
 * Last edited:
 *   2/1/2020, 3:29:38 PM
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

#include "Vec3.hpp"
#include "RenderObject.hpp"
#include "Camera.hpp"

namespace RayTracer {
    class RenderWorld {
        private:
            /* List that stores the objects in this world. */
            std::vector<RenderObject*> objects;
            /* List that stores all light objects in the world. */
            std::vector<int*> lights;

            /* Deepcopies the different vectors*/
            template <typename T> void deepcopy(std::vector<T*>& target, const std::vector<T*>& source) const;
        public:
            /* The RenderWorld class describes the world, and serves as the root for the object hiearchy. */
            RenderWorld();
            /* Copy constructor for the RenderWorld class. */
            RenderWorld(const RenderWorld& other);
            /* Move constructor for the RenderWorld class. */
            RenderWorld(RenderWorld&& other);
            ~RenderWorld();

            /* Adds a RenderObject to the world. Note that internally, this will copy the given object instead of reference it. */
            void add_object(RenderObject obj);
            /* Adds a RenderObject to the world. Note that anything added this way will be deallocated automatically. */
            void add_object(RenderObject* obj);
            /* Adds a RenderLight to the world. Note that internally, this will copy the given object instead of reference it. */
            void add_light(int light);
            /* Adds a RenderLight to the world. Note that anything added this way will be deallocated automatically. */
            void add_light(int* light);

            /* Returns a reference to an object for modification */
            RenderObject& get_object(int obj_index) const;
            /* Returns a reference to a camera object for modification */
            int& get_light(int light_index) const;

            /* Returns the number of objects defined in the RenderWorld. */
            const std::size_t get_object_count() const;
            /* Returns the number of light objects defined in the RenderWorld. */
            const std::size_t get_light_count() const;

            /* Computes the colour of a shot ray. The depth variable makes sure that, in the case of very reflective surfaces, we only do this at max 50 times. */
            Vec3 bounce_ray(const Ray& ray, int depth=0) const;
            /* Renders one pixel instead by shooting rays and everything. The optional cam_index can be used to render from another camera if there are multiple given. The index is equal to the order the cameras were added. */
            Vec3 render_pixel(int x, int y, const Camera& cam) const;

            /* Copy assignment operator for the RenderWorld class. */
            RenderWorld& operator=(RenderWorld other);
            /* Move assignment operator for the RenderWorld class. */
            RenderWorld& operator=(RenderWorld&& other);

            /* Swap operator for the RenderWorld class. */
            friend void swap(RenderWorld& first, RenderWorld& second);
    };
}

#endif