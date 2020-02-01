/* RAY TRACER.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 1:00:17 PM
 * Last edited:
 *   2/1/2020, 5:48:37 PM
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

#include <string>

#include "Frames.hpp"
#include "RenderWorld.hpp"
#include "Camera.hpp"

namespace RayTracer {
    class Renderer {
        public:
            int n_threads;
            int batch_size;
            bool show_progressbar;

            /* The Renderer class provides a more structures split between the prepration of the rendering and the rendering itself. */
            Renderer(int num_of_threads, int batch_size, bool show_progressbar);

            /* The render function renders a given world for you, either threaded or non-threaded (depends on how it is compiled) */
            Image render(RenderWorld* world, Camera* cam);

            /* This render function renders a whole series of Images. Before rendering a new one, the renderworld.update() method is called to update all objects. */
            void render_animation(RenderWorld* world, Camera* cam, Frames& out);
    };
}

#endif