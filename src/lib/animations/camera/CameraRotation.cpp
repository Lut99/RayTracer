/* CAMERA ROTATION.cpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 4:55:47 PM
 * Last edited:
 *   2/1/2020, 7:51:25 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The CameraRotation class models the movement of a Camera that circles
 *   around a point. In this animation, the Camera looks at the same point
 *   throughout the animation; just the lookfrom is changed. This
 *   particular file is the implementation file for CameraRotation.hpp.
**/

#include <math.h>

#include "../../include/animations/camera/CameraRotation.hpp"

using namespace std;
using namespace RayTracer;

double CameraRotation::compute_speed(std::chrono::seconds circle_time) {
    double circle_dist = 2 * M_PI;
    return circle_dist / ((double) circle_time.count());
}

CameraRotation::CameraRotation(Camera* target_cam, chrono::seconds circle_time)
    : CameraMovement(target_cam, rotation),
    loop_time(circle_time),
    speed(this->compute_speed(circle_time))
{
    // Compute the start angle
    Vec3 lookat = this->cam_target->lookat;
    Vec3 lookfrom = this->cam_target->lookfrom;
    Vec3 up = Vec3(lookat.x, lookfrom.y, lookat.z) - lookat;
    this->start = acosf64(dot(up, lookfrom) / (up.length() * lookfrom.length()));
}

void CameraRotation::update(chrono::milliseconds time_passed) {
    Vec3 lookfrom = this->cam_target->lookfrom;
    Vec3 lookat = this->cam_target->lookat;
    Vec3 up = Vec3(lookat.x, lookfrom.y, lookat.z) - lookat;

    // Fetch the current 'angle'
    double angle = acosf64(dot(up, lookfrom) / (up.length() * lookfrom.length()));

    // Rotate the vector by speed radians
    double speed = this->speed * chrono::duration_cast<chrono::seconds>(time_passed).count();

    // Rotate the circle_from vector
    Vec3 circle_from = lookfrom - Vec3(lookat.x, lookfrom.y, lookat.z);
    Vec3 rot_circle_from = Vec3(
        lookat.x + cosf64(angle + speed) * circle_from.length(),
        lookfrom.y,
        lookat.z + sinf64(angle + speed) * circle_from.length()
    );

    // Set the camera lookfrom to this new position
    this->cam_target->lookfrom = rot_circle_from + Vec3(lookat.x, lookfrom.y, lookat.z);

    // Recompute the camera variables
    this->cam_target->recompute();
}