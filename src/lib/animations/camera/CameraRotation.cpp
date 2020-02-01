/* CAMERA ROTATION.cpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 4:55:47 PM
 * Last edited:
 *   2/1/2020, 5:39:50 PM
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
{}

void CameraRotation::update(chrono::milliseconds time_passed) {
    Vec3 lookfrom = this->cam_target->lookfrom;
    Vec3 circle_center = Vec3(this->cam_target->lookat);
    circle_center.y = lookfrom.y;

    Vec3 on_circle = lookfrom - circle_center;

    // Rotate the vector by speed radians
    double speed = this->speed * chrono::duration_cast<chrono::seconds>(time_passed).count();
    Vec3 new_on_circle = Vec3(
        on_circle.x * cosf64(speed) - on_circle.y * sinf64(speed),
        on_circle.y,
        on_circle.z * sinf64(speed) + on_circle.y * cosf64(speed)
    );

    // Compute the difference vector
    Vec3 circle_diff = new_on_circle - on_circle;

    // Set the camera lookfrom to this new position
    this->cam_target->lookfrom += circle_diff;

    // Recompute the camera variables
    this->cam_target->recompute();
}