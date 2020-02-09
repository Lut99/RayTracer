/* CAMERA.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 3:23:14 PM
 * Last edited:
 *   2/9/2020, 1:14:51 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Camera class acts as the viewport, and therefore holds information
 *   about the viewport's location and rotation. Additionally, it handles
 *   the anti-aliasing. This particular file is the implementation file for
 *   Camera.hpp.
**/

#include <iostream>
#define _USE_MATH_DEFINES
#include <math.h>

#include "include/JSONExceptions.hpp"
#include "include/ProgressBar.hpp"
#include "include/Random.hpp"
#include "include/Camera.hpp"

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


Camera::Camera(Vec3 lookfrom, Vec3 lookat, Vec3 up, double vfov, double aperture, int screen_width, int screen_height, int rays_per_pixel, bool correct_gamma)
    : width(screen_width),
    height(screen_height),
    rays(rays_per_pixel),
    gamma(correct_gamma),
    lookfrom(lookfrom),
    lookat(lookat),
    up(up),
    vfov(vfov),
    aperture(aperture)
{
    this->recompute();

    this->animation = nullptr;
}
Camera::Camera(Vec3 lookfrom, Vec3 lookat, Vec3 up, double vfov, double aperture)
    : lookfrom(lookfrom),
    lookat(lookat),
    up(up),
    vfov(vfov),
    aperture(aperture)
{
    this->animation = nullptr;
}



Ray Camera::get_ray(int x, int y) const {
    // Compute u and v
    double s, t;
    if (this->rays > 1) {
        s = double(x + random_double()) / double(this->width);
        t = double(this->height - 1 - y + random_double()) / double(this->height);
    } else {
        s = double(x) / double(this->width);
        t = double(this->height - 1 - y) / double(this->height);
    }

    // Return a ray shot through this pixel
    Vec3 rd = this->lens_radius * random_in_unit_disc();
    Vec3 offset = this->u * rd.x + this->v * rd.y;
    return Ray(this->origin + offset, this->lower_left_corner + s * this->horizontal + t * this->vertical - this->origin - offset);
}



void Camera::recompute() {
    double ratio = (double) this->width / (double) this->height;
    double focus_dist = (this->lookfrom - this->lookat).length();
    this->lens_radius = this->aperture / 2;

    double theta = this->vfov * M_PI / 180;
    double half_height = tan(theta/2);
    double half_width = ratio * half_height;

    this->origin = this->lookfrom;
    this->w = (this->lookfrom - this->lookat).normalize();
    this->u = cross(this->up, this->w).normalize();
    this->v = cross(this->w, this->u);

    this->lower_left_corner = this->origin - half_width * focus_dist * this->u - half_height * focus_dist *  this->v - focus_dist * this->w;
    this->horizontal = 2 * half_width * focus_dist * this->u;
    this->vertical = 2 * half_height * focus_dist * this->v;
}



void Camera::set_animation(CameraMovement* animation) {
    this->animation = animation;
    // Also recompute the animation based on this object
    this->animation->recompute(this);
}

void Camera::update(std::chrono::milliseconds time_passed){
    // Only if there is one, update the animation
    if (this->animation != nullptr) {
        this->animation->update(time_passed, this);
    }
}



json Camera::to_json() const {
    json j;
    j["lookat"] = this->lookat.to_json();
    j["lookfrom"] = this->lookfrom.to_json();
    j["up"] = this->up.to_json();
    j["vfov"] = this->vfov;
    j["aperture"] = this->aperture;
    if (this->animation != nullptr) {
        j["animation"] = this->animation->to_json();
    }
    return j;
}

Camera* Camera::from_json(nlohmann::json json_obj) {
    // Check if it is an object
    if (!json_obj.is_object()) {
        throw InvalidTypeException("Camera", json::object().type_name(), json_obj.type_name());
    }

    // Next, check if the required fields are present
    if (json_obj["lookat"].is_null()) {
        throw MissingFieldException("Camera", "lookat");
    }
    if (json_obj["lookfrom"].is_null()) {
        throw MissingFieldException("Camera", "lookfrom");
    }
    if (json_obj["up"].is_null()) {
        throw MissingFieldException("Camera", "up");
    }
    if (json_obj["vfov"].is_null()) {
        throw MissingFieldException("Camera", "vfov");
    }
    if (json_obj["aperture"].is_null()) {
        throw MissingFieldException("Camera", "aperture");
    }

    // Parse the objects
    Vec3 lookat = Vec3::from_json(json_obj["lookat"]);
    Vec3 lookfrom = Vec3::from_json(json_obj["lookfrom"]);
    Vec3 up = Vec3::from_json(json_obj["up"]);
    double vfov, aperture;
    try {
        vfov = json_obj["vfov"].get<double>();
        aperture = json_obj["aperture"].get<double>();
    } catch (nlohmann::detail::type_error& e) {
        throw InvalidFieldFormat("Camera", "vfov or aperture", "double", json_obj[0].type_name());
    }
    // Only parse the animation if it is present
    CameraMovement* animation = nullptr;
    if (!json_obj["animation"].is_null()) {
        animation = CameraMovement::from_json(json_obj["animation"]);
    }
    
    // Put everything in the camera
    Camera* to_return = new Camera(lookfrom, lookat, up, vfov, aperture);
    if (animation != nullptr) {
        to_return->set_animation(animation);
    }

    // Return it
    return to_return;
}