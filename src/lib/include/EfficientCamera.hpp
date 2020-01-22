/* EFFICIENT CAMERA.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 9:13:40 PM
 * Last edited:
 *   1/22/2020, 9:36:04 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The EfficientCamera class is a multi-threaded and possibly
 *   GPU-rendered version of the normal camera. This particular file is the
 *   header file for EfficientCamera.cpp.
**/

#ifndef EFFICIENTCAMERA_HPP
#define EFFICIENTCAMERA_HPP

#include "Image.hpp"
#include "RenderObjectCollection.hpp"
#include "Ray.hpp"
#include "Vec3.hpp"

namespace RayTracer {
    class EfficientCamera {
        public:
            const int width;
            const int height;
            const int rays;
            const bool gamma;
            const int n_threads;

            Vec3 lower_left_corner;
            Vec3 horizontal;
            Vec3 vertical;
            Vec3 origin;

            /* The camera class holds information about the viewport of the scene. Note that for now, everything is set, but I suspect that may change later. */
            EfficientCamera(int screen_width, int screen_height, int rays_per_pixel, bool correct_gamma, int num_of_threads);

            /* Returns a ray through given u and v through the pixel grid */
            Ray get_ray(double u, double v) const;

            /* Renders a frame from the current camera position */
            Image render(const RenderObjectCollection& world);  
    };
}

#endif