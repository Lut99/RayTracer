/* EFFICIENT CAMERA.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 9:13:40 PM
 * Last edited:
 *   1/23/2020, 3:55:11 PM
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

#include "Camera.hpp"

namespace RayTracer {
    class EfficientCamera: public Camera {
        public:
            const int n_threads;

            /* The camera class holds information about the viewport of the scene. Note that for now, everything is set, but I suspect that may change later. */
            EfficientCamera(Vec3 lookfrom, Vec3 lookat, Vec3 up, double vfov, int screen_width, int screen_height, int rays_per_pixel, bool show_progressbar, bool correct_gamma, int number_of_threads);

            /* Renders a frame from the current camera position */
            virtual Image render(const RenderObject& world) const;
    };
}

#endif