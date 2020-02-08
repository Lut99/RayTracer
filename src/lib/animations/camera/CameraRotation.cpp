/* CAMERA ROTATION.cpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 4:55:47 PM
 * Last edited:
 *   2/8/2020, 11:38:22 PM
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

#include "../../include/JSONExceptions.hpp"
#include "../../include/animations/camera/CameraRotation.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


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



json CameraRotation::to_json() const {
    json j;
    j["type"] = (unsigned long) this->type;
    j["cam_type"] = (unsigned long) this->cam_type;
    j["loop_time"] = this->loop_time.count();
    return j;
}
CameraRotation* CameraRotation::from_json(json json_obj) {
    // Check if the object has an object type
    if (!json_obj.is_object()) {
        throw InvalidTypeException("CameraRotation", json::object().type_name(), json_obj.type_name());
    }

    // Check if the required field exist
    if (json_obj["loop_time"].is_null()) {
        throw MissingFieldException("CameraRotation", "loop_time");
    }

    // Try to parse the required fields
    chrono::seconds loop_time;
    try {
        loop_time = chrono::seconds(json_obj[0].get<long>());
    } catch (nlohmann::detail::type_error& e) {
        throw InvalidTypeException("CameraRotation", "unsigned long", json_obj[0].type_name());
    }
    return new CameraRotation(nullptr, loop_time);
}