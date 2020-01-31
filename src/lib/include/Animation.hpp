/* ANIMATION.hpp
 *   by Lut99
 *
 * Created:
 *   1/31/2020, 2:25:18 PM
 * Last edited:
 *   1/31/2020, 3:09:29 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Animation class wraps the Image class, and is meant to be able to
 *   render a little movie. In contrast to a simple list of Images, the
 *   animation also holds information about the framerate and number of
 *   frames, and can dynamically output frames while rendering to reduce
 *   the memory impact. This particular file is the header file for
 *   Animation.cpp.
**/

#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "Image.hpp"

namespace RayTracer {
    class Animation {
        private:
            Image* current_frame;
            Image** frames;

            std::size_t frame_index;
            std::string temp_dir;
            bool dynamic_writing;

        public:
            int width;
            int height;
            int n_frames;
            int fps;

            /* The animation class is used to render multiple frames, and output them as a movie. The dynamic_write boolean determines if the entire animation is saved in memory, and then to the disk (false) or if each image should be written to disk as soon it is rendered (true). Note that .to_mp4() still has to be called to create an mp4 from the images. */
            Animation(int width, int height, int num_of_frames, int framerate, std::string temp_dir, bool dynamic_write);
            /* Copy operator for the Animation class. */
            Animation(const Animation& other);
            ~Animation();

            /* Finishes up this frame, and moves onto the next. */
            void next();

            /* Returns a row within the current frame. */
            ImageRow operator[](int index);
            /* Copy operator for the Animation class. */
            Animation& operator=(const Animation& other);

            /* Saves the internal buffer to disk if this isn't done dynamically, and then combines all images into a .mp4. */
            void to_mp4(std::string path);
    };
}

#endif