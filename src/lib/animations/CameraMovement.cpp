/* CAMERA MOVEMENT.cpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 4:48:11 PM
 * Last edited:
 *   2/8/2020, 11:14:25 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The CameraMovement class describes how the Camera can move around. It
 *   is in itself another animation baseclass, as there are multiple types
 *   of camera movements. This particular file is the implementation for
 *   CameraMovement.hpp.
**/

#include <stdexcept>

#include "../include/animations/CameraMovement.hpp"

using namespace std;
using namespace RayTracer;

CameraMovement::CameraMovement(Camera* target_cam, CameraMovementType movement_type)
    : RenderAnimation(nullptr, camera_movement),
    cam_target(target_cam),
    type(movement_type)
{}

void CameraMovement::update(chrono::milliseconds time_passed) {
    throw runtime_error("Function CameraMovement::update(chrono::milliseconds time_passed) is not overridden.");
}
