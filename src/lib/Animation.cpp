/* ANIMATION.cpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 2:15:15 PM
 * Last edited:
 *   2/1/2020, 5:38:44 PM
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
 *   particular file is the implementation file for Animation.hpp.
**/

#include <stdexcept>

#include "include/Animation.hpp"

using namespace std;
using namespace RayTracer;

Animation::Animation(RenderObject* target_object, AnimationType animation_type)
    : target(target_object),
    type(animation_type)
{}

void Animation::update(chrono::milliseconds time_passed) {
    throw runtime_error("Function Animation::update(chrono::milliseconds time_passed) is not overridden.");
}
