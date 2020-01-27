/* CAMERA.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 3:23:28 PM
 * Last edited:
 *   1/27/2020, 3:20:37 PM
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
#ifdef CAMERA_THREADS
#include "ThreadPool.hpp"
#endif

namespace RayTracer {
    class RenderWorld;
    class ThreadPool;

    class Camera {
        public:
            const int width;
            const int height;
            const int rays;
            const bool gamma;

            Vec3 lower_left_corner;
            Vec3 horizontal;
            Vec3 vertical;
            Vec3 origin;

            /* The camera class holds information about the viewport of the scene. Note that for now, everything is set, but I suspect that may change later. */
            Camera(Vec3 lookfrom, Vec3 lookat, Vec3 up, double vfov, int screen_width, int screen_height, int rays_per_pixel, bool correct_gamma);

            /* Returns a ray through given u and v through the pixel grid */
            Ray get_ray(int x, int y) const;
    };
}

#endif