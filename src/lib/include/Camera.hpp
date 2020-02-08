/* CAMERA.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 3:23:28 PM
 * Last edited:
 *   2/8/2020, 11:04:19 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Camera class acts as the viewport, and therefore holds information
 *   about the viewport's location and rotation. Additionally, it handles
 *   the anti-aliasing. This particular file is the header file for
 *   Camera.cpp.
**/

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "json.hpp"
#include "Image.hpp"
#include "Vec3.hpp"
#include "Ray.hpp"

namespace RayTracer {
    class RenderWorld;
    class ThreadPool;

    class Camera {
        public:
            /* My added variables */
            int width;
            int height;
            int rays;
            bool gamma;

            /* Setting variables */
            Vec3 lookfrom;
            Vec3 lookat;
            Vec3 up;
            double vfov;
            double aperture;

            /* Computed variables */
            Vec3 lower_left_corner;
            Vec3 horizontal;
            Vec3 vertical;
            Vec3 origin;
            Vec3 u, w, v;
            double lens_radius;

            /* The camera class holds information about the viewport of the scene. */
            Camera(Vec3 lookfrom, Vec3 lookat, Vec3 up, double vfov, double aperture, int screen_width, int screen_height, int rays_per_pixel, bool correct_gamma);
            /* The camera class holds information about the viewport of the scene. This overload only stores some basic variables, and does not recompute the rest yet. */
            Camera(Vec3 lookfrom, Vec3 lookat, Vec3 up, double vfov, double aperture);

            /* Recomputes the Camera values. This is useful after the given variables are altered after construction. */
            void recompute();

            /* Returns a ray through given u and v through the pixel grid */
            Ray get_ray(int x, int y) const;

            /* Returns a json object with the properties of this Camera object. */
            virtual nlohmann::json to_json() const;
            /* Returns a new Camera object from given json object. Note that this new object will have to be deallocated. */
            static Camera* from_json(nlohmann::json json_obj);
    };
}

#endif