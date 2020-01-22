/* RENDER WORLD.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 4:10:56 PM
 * Last edited:
 *   1/22/2020, 4:34:12 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The RenderWorld class is a modified RenderObjectCollection class (and
 *   inherits from it). This class is meant to be the root of the object
 *   tree, and is the only class that will actively diffure (since it has
 *   access to all other objects). This particular file is the header file
 *   for RenderWorld.cpp.
**/

#ifndef RENDERWORLD_HPP
#define RENDERWORLD_HPP

#include "Camera.hpp"
#include "RenderObjectCollection.hpp"

namespace RayTracer {
    class Camera;

    class RenderWorld: public RenderObjectCollection {
        private:
            Camera* camera;
        public:
            /* The RenderWorld is a special case of a RenderObjectCollection that is meant to be the root of the object tree. It will actively do more advanced rendering such as diffusing, because it has access to all objects. */
            RenderWorld(Camera& cam, std::vector<RenderObject*> objects);

            /* Override for RenderObjectCollection's colour method, with diffusing. */
            virtual Vec3 colour(const HitRecord& record) const;
    };
}

#endif