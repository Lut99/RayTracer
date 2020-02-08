/* RENDER ANIMATION.cpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 2:15:15 PM
 * Last edited:
 *   2/8/2020, 11:13:14 PM
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
 *   particular file is the implementation file for RenderAnimation.hpp.
**/

#include <stdexcept>

#include "include/RenderAnimation.hpp"

using namespace std;
using namespace RayTracer;

RenderAnimation::RenderAnimation(RenderObject* target_object, RenderAnimationType animation_type)
    : target(target_object),
    type(animation_type)
{}

void RenderAnimation::update(chrono::milliseconds time_passed) {
    throw runtime_error("Function RenderAnimation::update(chrono::milliseconds time_passed) is not overridden.");
}
