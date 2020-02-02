/* CAMERA ROTATION.cpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 4:55:47 PM
 * Last edited:
 *   2/2/2020, 5:49:23 PM
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
    Vec3 up = Vec3(1, 0, 0);

    // Fetch the current 'angle'
    Vec3 lookfrom_plane = Vec3(
        lookfrom.x - lookat.x,
        0,
        lookfrom.y - lookat.y
    );
    double angle = acosf64(dot(up, lookfrom_plane) / (up.length() * lookfrom_plane.length()));

    // Rotate the vector by speed radians
    double speed = this->speed * chrono::duration_cast<chrono::seconds>(time_passed).count();

    // Compute the new lookfrom_plane
    Vec3 rot_lookfrom_plane = Vec3(
        lookfrom_plane.x + cosf64(angle + speed) * lookfrom_plane.length(),
        0,
        lookfrom_plane.z + sinf64(angle + speed) * lookfrom_plane.length()
    );

    // Set the camera lookfrom to this new position
    this->cam_target->lookfrom = Vec3(
        rot_lookfrom_plane.x + lookat.x,
        lookfrom.y,
        rot_lookfrom_plane.z + lookat.z
    );

    // Recompute the camera variables
    this->cam_target->recompute();
}