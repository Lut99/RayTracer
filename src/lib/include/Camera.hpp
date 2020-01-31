/* CAMERA.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 3:23:28 PM
 * Last edited:
 *   1/31/2020, 1:30:48 PM
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
    };
}

#endif