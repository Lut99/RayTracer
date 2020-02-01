/* FRAMES.cpp
 *   by Lut99
 *
 * Created:
 *   1/31/2020, 2:25:02 PM
 * Last edited:
 *   2/1/2020, 5:56:12 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Frames class wraps the Image class, and is meant to be able to
 *   render a little movie. In contrast to a simple list of Images, the
 *   animation also holds information about the framerate and number of
 *   frames, and can dynamically output frames while rendering to reduce
 *   the memory impact. This particular file is the implementation for
 *   Frames.hpp.
**/

#include <iostream>
#include <fstream>
#include <cstring>
#include <cerrno>

#include "include/Frames.hpp"

using namespace std;
using namespace RayTracer;

Frames::Frames(int width, int height, int num_of_frames, int framerate, std::string temp_dir, bool dynamic_write)
    : width(width),
    height(height),
    n_frames(num_of_frames),
    fps(framerate),
    frame_index(0),
    temp_dir(temp_dir),
    dynamic_writing(dynamic_write)
{
    // Touch a file in the temp dir to make sure that the director exists
    ofstream temp_check(temp_dir + "/out1.png");
    if (!temp_check.is_open()) {
        cerr << "temp_dir existance check failed: " << strerror(errno) << endl;
        exit(-1);
    }
    temp_check.close();

    // Initialize the first frame
    this->current_frame = new Image(this->width, this->height);

    // If everything should be kept in memory, allocate and sync the frames list as well
    if (!this->dynamic_writing) {
        this->frames = new Image*[this->n_frames];
        this->frames[this->frame_index] = this->current_frame;
    } 
}

Frames::Frames(const Frames& other)
    : width(other.width),
    height(other.height),
    n_frames(other.n_frames),
    fps(other.fps),
    frame_index(other.frame_index),
    temp_dir(other.temp_dir),
    dynamic_writing(other.dynamic_writing)
{
    // Depending on whether we write dynamically, copy the necessary data
    if (this->dynamic_writing) {
        // Copy the current_frame, done
        this->current_frame = new Image(*other.current_frame);
    } else {
        // Copy their entire this->frames list
        this->frames = new Image*[this->n_frames];
        for (std::size_t i = 0; i < this->frame_index; i++) {
            this->frames[i] = new Image(*other.frames[i]);
        }
        // Update the current_frame
        this->current_frame = this->frames[frame_index];
    }
}

Frames::Frames(Frames&& other)
    : width(other.width),
    height(other.height),
    n_frames(other.n_frames),
    fps(other.fps),
    frame_index(other.frame_index),
    temp_dir(other.temp_dir),
    dynamic_writing(other.dynamic_writing)
{
    // Copy the correct pointers
    if (this->dynamic_writing) {
        this->current_frame = other.current_frame;
    } else {
        this->frames = other.frames;
        this->current_frame = this->frames[this->frame_index];
    }

    // Overwrite the other pointer with null
    other.frames = nullptr;
    other.current_frame = nullptr;
}

Frames::~Frames() {
    // Deallocate either everything or only the current frame
    if (!this->dynamic_writing) {
        for (std::size_t i = 0; i <= this->frame_index; i++) {
            delete this->frames[i];
        }
        // Delete the list as well
        delete[] this->frames;
    } else {
        delete this->current_frame;
    }
}



void Frames::next() {
    // Do nothing if we're already at the last frame
    if (this->frame_index == this->n_frames - 1) {
        return;
    }

    // Depending on whether we should keep everything in memory, write current image to disk and delete it
    if (this->dynamic_writing) {
        this->current_frame->to_png(this->temp_dir + "/out" + to_string(this->frame_index) + ".png");

        delete this->current_frame;
    }

    // Create a new frame
    this->frame_index++;
    this->current_frame = new Image(this->width, this->height);

    // If we should keep everything in memory, update the frames list
    if (!this->dynamic_writing) {
        this->frames[this->frame_index] = this->current_frame;
    }
}

ImageRow Frames::operator[](int index) {
    // Wrap the operator of the internal current frame
    return this->current_frame->operator[](index);
}

void Frames::to_mp4(string path) {
    // If we haven't, write all images to file
    if (!this->dynamic_writing) {
        for (std::size_t i = 0; i < this->frame_index; i++) {
            this->frames[i]->to_png(this->temp_dir + "/out" + to_string(i) + ".png");
        }
    }

    // Release ffmpeg on the files
    string command = "ffmpeg ";
    command += "-r " + to_string(this->fps);
    command += " -f image2";
    command += " -s " + to_string(this->width) + "x" + to_string(this->height);
    command += " -i \"" + this->temp_dir + "/out%d.png\"";
    command += " -vcodec libx264";
    command += " -crf 25";
    command += " -pix_fmt yuv420p";
    command += " \"" + path + "\"";

    cout << "Running ffmpeg with command: '" + command + "'" << endl;

    int res = system(command.c_str());
}

const std::size_t Frames::get_frame_index() const {
    return this->frame_index;
}