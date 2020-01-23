/* CAMERA.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 3:23:28 PM
 * Last edited:
 *   1/23/2020, 8:34:47 AM
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
#include "RenderObjectCollection.hpp"
#include "Vec3.hpp"
#include "Ray.hpp"

namespace RayTracer {
    class RenderWorld;

    class Camera {
        public:
            const int width;
            const int height;
            const int rays;
            const bool progress;
            const bool gamma;

            Vec3 lower_left_corner;
            Vec3 horizontal;
            Vec3 vertical;
            Vec3 origin;

            /* The camera class holds information about the viewport of the scene. Note that for now, everything is set, but I suspect that may change later. */
            Camera(int screen_width, int screen_height, int rays_per_pixel, bool show_progressbar, bool correct_gamma);

            /* Returns a ray through given u and v through the pixel grid */
            Ray get_ray(double u, double v) const;
            /* Computes the colour of a shot ray. The depth variable makes sure that, in the case of very reflective surfaces, we only do this at max 50 times. */
            Vec3 shoot_ray(const Ray& ray, const RenderObject& world, int depth=0) const;

            /* Renders a frame from the current camera position */
            virtual Image render(const RenderObject& world) const;
            /* Renders one pixel from the scene. */
            Vec3 render_pixel(int x, int y, const RenderObject& world) const;
    };
}

#endif