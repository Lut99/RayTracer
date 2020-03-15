/* RENDER ANIMATION.hpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 2:15:38 PM
 * Last edited:
 *   2/9/2020, 1:57:55 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The RenderAnimation class defines a movement or change of texture on an
 *   object. They can be separately added to the RenderWorld with the
 *   target RenderObject in mind, and will change properties that are
 *   available to the RenderObject. Note that some animations, such as
 *   texture change, cannot be added for each object. Also, the special
 *   CameraAnimation child class can mutate the camera mid-render. This
 *   particular file is the header file for RenderAnimation.cpp.
**/

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include <chrono>

#include "json.hpp"
#include "RenderObject.hpp"

namespace RayTracer {
    class RenderObject;

    enum RenderAnimationType {};

    class RenderAnimation {
        protected:
            /* Note that the baseclass is not meant to be called directly, but only from child classes. */
            RenderAnimation(RenderAnimationType animation_type);

            /* This function compiles RenderAnimation-general properies to a given json object. */
            virtual void baseclass_to_json(nlohmann::json& json_obj) const;
        public:
            /* Describes the subtype of the animation. */
            const RenderAnimationType type;

            /* Virtual deconstructor for the RenderAnimation class. */
            virtual ~RenderAnimation() = default;

            /* Virtual function that lets the derived classes compute something before the first update() is called. If child classes do not use this, they should implement a dummy function. */
            virtual void recompute(RenderObject* target) = 0;
            /* Virtual for the update() function. After each frame, this is called in the derived class to update the object it animates. The number is the amount of milliseconds since the last frame in the movie (so not the rendertime), and the target is the object this animation performs it's thing on. */
            virtual void update(std::chrono::milliseconds time_passed, RenderObject* target) = 0;

            /* Returns a json object based on this RenderAnimation. */
            virtual nlohmann::json to_json() const;
            /* Returns a child of the RenderAnimation from a json object. Note that the returned object is allocated and has to be deallocated later on. */
            static RenderAnimation* from_json(nlohmann::json json_obj);
    };
}

#endif