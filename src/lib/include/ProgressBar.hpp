/* PROGRESS BAR.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 4:45:31 PM
 * Last edited:
 *   05/03/2020, 18:13:56
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The ProgessBar file specializes at showing neat progress bars, and is 
 *   usable in about any project. Is derived from Lut99's Python
 *   ProgressBar implementation. This file is the header file for 
 *   ProgressBar.cpp.
**/

#ifndef PROGRESSBAR_HPP
#define PROGRESSBAR_HPP

#include <chrono>
#include <string>

namespace std {
    class ProgressBar {
        private:
            bool done;

            long progress;
            long min;
            long max;
            
            std::string left;
            std::string right;

            int width;
            int bar_width;
            int bar_inner_width;

            bool var_width;

            chrono::milliseconds last_draw;
            chrono::milliseconds interval;

            /* Initializes the ProgressBar beyond the header-dependent functions. */
            void init();

            /* Draws the progress bar. Is called once every this->interval milliseconds from update(). */
            void draw() const;

            /* Is called when the progressbar completes. */
            void win();

            /* Returns the width of the terminal */
            int resize_to_term_width() const;
        public:
            /* The ProgressBar class is useful for visualizing the progress of a file. Displays a bar that slowly fills and a percentage. By default, takes the full width of the console with no text left or right to the bar. The minimum value is 0, and the maximum is 100. The default interval is 500 milliseconds. */
            ProgressBar();
            /* The ProgressBar class is useful for visualizing the progress of a file. Displays a bar that slowly fills and a percentage. */
            ProgressBar(long min_value, long max_value);
            /* The ProgressBar class is useful for visualizing the progress of a file. Displays a bar that slowly fills and a percentage. */
            ProgressBar(long min_value, long max_value, std::string left_text, std::string right_text);
            /* The ProgressBar class is useful for visualizing the progress of a file. Displays a bar that slowly fills and a percentage. */
            ProgressBar(long min_value, long max_value, std::string left_text, std::string right_text, int width);
            /* The ProgressBar class is useful for visualizing the progress of a file. Displays a bar that slowly fills and a percentage. */
            ProgressBar(long min_value, long max_value, std::string left_text, std::string right_text, int width, chrono::milliseconds update_interval);

            /* Updates the progress bar by a specified amount. Note that this does not necessarily call 'draw' as well - that is interval based */
            void update(long amount=1);
            /* Sets the progress bar to a specified amount. Note that this does not necessarily call 'draw' as well - that is interval based */
            void set(long value);
    };
}

#endif