/* RAY TRACER.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:00:17 PM
 * Last edited:
 *   1/31/2020, 2:17:34 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file is the renderer for the RayTracer. This new version includes the
 *   complete 'Raytracing in One Weekend' tutorial, but then multithreaded (if
 *   compiled that way). Additionally, it will also be able to handle moving
 *   objects and rendering a movie. This particular file is the header file
 *   for Renderer.cpp.
**/

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "RenderWorld.hpp"
#include "Camera.hpp"

namespace RayTracer {
    class Renderer {
        public:
            int n_threads;
            int batch_size;
            bool show_progressbar;

            Renderer(int num_of_threads, int batch_size, bool show_progressbar);

            /* The render function renders a given world for you, either threaded or non-threaded (depends on how it is compiled) */
            Image render(RenderWorld* world, Camera* cam);
    };
}

#endif