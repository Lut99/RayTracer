/* CAMERA ROTATION.cpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 4:55:47 PM
 * Last edited:
 *   2/3/2020, 4:58:26 PM
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
    return circle_dist / (((double) circle_time.count()) * 1000);
}

CameraRotation::CameraRotation(Camera* target_cam, chrono::seconds circle_time)
    : CameraMovement(target_cam, rotation),
    loop_time(circle_time),
    speed(this->compute_speed(circle_time))
{
    // Compute the start positions
    Vec3 lookat = this->cam_target->lookat;
    Vec3 lookfrom = this->cam_target->lookfrom;
    Vec3 lookfrom_circle(lookfrom.x, 0, lookfrom.z);

    this->center = Vec3(lookat.x, 0, lookat.z);
    this->angle = acosf64(dot(Vec3(0, 0, 1), (lookfrom_circle - center).normalize()));
    this->radius = (lookfrom_circle - this->center).length();
}

void CameraRotation::update(chrono::milliseconds time_passed) {
    // Set the camera lookfrom to this new position
    this->cam_target->lookfrom = Vec3(
        this->center.x + cosf64(this->angle) * this->radius,
        this->cam_target->lookfrom.y,
        this->center.z + sinf64(this->angle) * this->radius
    );

    this->angle += this->speed * time_passed.count();
    while (this->angle >= 2 * M_PI) {
        this->angle -= 2 * M_PI;
    }

    // Recompute the camera variables
    this->cam_target->recompute();
}