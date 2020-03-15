/* RANDOM SCENE.hpp
 *   by Lut99
 *
 * Created:
 *   1/31/2020, 12:12:39 PM
 * Last edited:
 *   07/03/2020, 11:48:57
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The RandomScene class is a derived class of a RenderWorld. Instead of
 *   letting the user specify the object themselves, this class generates a
 *   random world upon initialization as specified of the end of the
 *   tutorial. This particular file is the header file for RandomScene.cpp.
**/

#ifndef RANDOMSCENE_HPP
#define RANDOMSCENE_HPP

#include "RenderWorld.hpp"

namespace RayTracer {
    class RandomScene: public RenderWorld {
        public:
            /* We only need to overload the constructor, as this initializes the random world. */
            RandomScene();
    };
}

#endif