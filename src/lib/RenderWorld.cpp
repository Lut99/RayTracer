/* RENDER WORLD.cpp
 *   by Lut99
 *
 * Created:
 *   1/27/2020, 2:30:39 PM
 * Last edited:
 *   1/27/2020, 3:27:54 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The RenderWorld class is representing the 3D world. This is where the
 *   computation happens like shooting rays and bouncing them, while the
 *   Camera handles only the viewport-related stuff. An additional
 *   advantage of the RenderWorld is the easier use with respect to the
 *   ThreadPool class. This particular file handles the implementation for
 *   RenderWorld.hpp.
**/

#include <stdexcept>

#include "include/RenderWorld.hpp"

using namespace std;
using namespace RayTracer;

RenderWorld::RenderWorld() {
    // Nothing much, really
}

RenderWorld::~RenderWorld() {
    // Undeclare everything in the vectors
    for (std::size_t i = 0; i < this->objects.size(); i++) {
        delete this->objects[i];
    }
    for (std::size_t i = 0; i < this->lights.size(); i++) {
        delete this->lights[i];
    }
}



void RenderWorld::add_object(RenderObject obj) {
    this->objects.push_back(new RenderObject(obj));
}

void RenderWorld::add_object(RenderObject* obj) {
    this->objects.push_back(obj);
}

void RenderWorld::add_light(int light) {

}

void RenderWorld::add_light(int* light) {
    
}

void RenderWorld::add_camera(Camera cam) {
    this->cameras.push_back(new Camera(cam));
}

void RenderWorld::add_camera(Camera* cam) {
    this->cameras.push_back(cam);
}



RenderObject& RenderWorld::get_object(int obj_index) const {
    // Check if not out of bounds
    if (obj_index < 0 || obj_index >= this->objects.size()) {
        throw out_of_range("Object index " + to_string(obj_index) + " is out of range for World with " + to_string(this->objects.size()) + " objects.");
    }

    // Return the object
    return *this->objects[obj_index];
}
int& RenderWorld::get_light(int light_index) const {
    // Check if not out of bounds
    if (light_index < 0 || light_index >= this->lights.size()) {
        throw out_of_range("Light index " + to_string(light_index) + " is out of range for World with " + to_string(this->lights.size()) + " lights.");
    }

    // Return the object
    return *this->lights[light_index];
}
Camera& RenderWorld::get_camera(int cam_index) const {
    // Check if not out of bounds
    if (cam_index < 0 || cam_index >= this->cameras.size()) {
        throw out_of_range("Camera index " + to_string(cam_index) + " is out of range for World with " + to_string(this->cameras.size()) + " cameras.");
    }

    // Return the object
    return *this->cameras[cam_index];
}




Vec3 RenderWorld::bounce_ray(const Ray& ray, int depth) const {
    // Find the hit with the smallest hit
    HitRecord record;
    double t_best = numeric_limits<double>::max();
    bool hit = false;
    for (std::size_t i = 0; i < this->objects.size(); i++) {
        if (this->objects[i]->hit(ray, 0.0, t_best, record)) {
            // Store this hit as the best one
            t_best = record.t;
            hit = true;
        }
    }

    // Check if a hit occured
    if (hit) {
        Ray scattered;
        Vec3 attenuation;
        if (depth < 50 && record.material->scatter(ray, record, attenuation, scattered)) {
            return attenuation * this->bounce_ray(scattered, depth + 1);
        } else {
            return Vec3(0, 0, 0);
        }
    } else {
        Vec3 unit = ray.direction.normalize();
        double t = 0.5 * (unit.y + 1.0);
        return (1.0 - t) * Vec3(1, 1, 1) + t * Vec3(0.5, 0.7, 1.0);
    }
}

Vec3 RenderWorld::render_pixel(int x, int y, int cam_index) const {
    // Check if the cam_index is within range
    if (cam_index < 0 || cam_index >= this->cameras.size()) {
        throw out_of_range("Camera index " + to_string(cam_index) + " is out of range for World with " + to_string(this->cameras.size()) + " cameras.");
    }

    Camera* cam = this->cameras[cam_index];
    Vec3 col;
    for (int r = 0; r < cam->rays; r++) {
        Ray ray = cam->get_ray(x, y);

        // Get the colour
        col += this->bounce_ray(ray);
    }

    // Compute the colour average
    Vec3 avg_col = col / cam->rays;
    if (cam->gamma) {
        // Gamma-correct the avg_colour
        return Vec3(sqrt(avg_col.x), sqrt(avg_col.y), sqrt(avg_col.z));
    } else {
        return avg_col;
    }
}