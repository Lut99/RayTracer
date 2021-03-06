/* FRAMES.hpp
 *   by Lut99
 *
 * Created:
 *   1/31/2020, 2:25:18 PM
 * Last edited:
 *   2/9/2020, 12:50:45 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Frames class wraps the Image class, and is meant to be able to
 *   render a little movie. In contrast to a simple list of Images, the
 *   animation also holds information about the framerate and number of
 *   frames, and can dynamically output frames while rendering to reduce
 *   the memory impact. This particular file is the header file for
 *   Frames.cpp.
**/

#ifndef FRAMES_HPP
#define FRAMES_HPP

#include "Image.hpp"

namespace RayTracer {
    class Frames {
        private:
            Image* current_frame;
            Image** frames;

            std::size_t frame_index;
            std::string temp_dir;
            bool dynamic_writing;

        public:
            const unsigned int width;
            const unsigned int height;
            const unsigned int n_frames;
            const unsigned int fps;

            /* The animation class is used to render multiple frames, and output them as a movie. The dynamic_write boolean determines if the entire animation is saved in memory, and then to the disk (false) or if each image should be written to disk as soon it is rendered (true). Note that .to_mp4() still has to be called to create an mp4 from the images. */
            Frames(unsigned int width, unsigned int height, unsigned int num_of_frames, unsigned int framerate, std::string temp_dir, bool dynamic_write);
            /* Copy constructor for the Frames class. */
            Frames(const Frames& other);
            /* Move constructor for the Frames class. */
            Frames(Frames&& other);
            ~Frames();

            /* Finishes up this frame, and moves onto the next. */
            void next();

            /* Returns a row within the current frame. */
            ImageRow operator[](int index);

            /* Returns a reference to the current frame */
            Image& get_current_frame();

            /* Saves the internal buffer to disk if this isn't done dynamically, and then combines all images into a .mp4. */
            void to_mp4(std::string path);

            /* Returns the current frame_index. Note that this is zero-indexed. */
            std::size_t get_frame_index() const;
    };
}

#endif