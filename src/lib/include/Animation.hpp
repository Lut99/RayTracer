/* ANIMATION.hpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 2:15:38 PM
 * Last edited:
 *   2/1/2020, 5:38:40 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Animation class defines a movement or change of texture on an
 *   object. They can be separately added to the RenderWorld with the
 *   target RenderObject in mind, and will change properties that are
 *   available to the RenderObject. Note that some animations, such as
 *   texture change, cannot be added for each object. Also, the special
 *   CameraAnimation child class can mutate the camera mid-render. This
 *   particular file is the header file for Animation.cpp.
**/

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <chrono>

#include "RenderObject.hpp"

namespace RayTracer {
    enum AnimationType {
        camera_movement
    };

    class Animation {
        protected:
            /* Pointer to the object this animation works on. */
            RenderObject* target;

            /* Note that the baseclass is not meant to be called directly, but only from child classes. */
            Animation(RenderObject* target_object, AnimationType animation_type);
        public:
            /* Describes the subtype of the animation. */
            const AnimationType type;

            /* Virtual for the update() function. After each frame, this is called in the derived class to update the object it animates. The number is the amount of milliseconds since the last frame in the movie (so not the rendertime). */
            virtual void update(std::chrono::milliseconds time_passed);
    };
}

#endif