/* PROGRESS BAR.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 4:44:56 PM
 * Last edited:
 *   1/22/2020, 6:11:07 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The ProgessBar file specializes at showing neat progress bars, and is
 *   usable in about any project. Is derived from Lut99's Python
 *   ProgressBar implementation. This file is the implementation file for
 *   ProgressBar.hpp.
**/

#include <sys/ioctl.h>
#include <unistd.h>

#include <stdexcept>
#include <iostream>

#include "include/ProgressBar.hpp"

#define DEFAULT_MIN 0
#define DEFAULT_MAX 0
#define DEFAULT_LEFT string("")
#define DEFAULT_RIGHT string("")
#define DEFAULT_WIDTH -1
#define DEFAULT_INTERVAL chrono::milliseconds(500)

using namespace std;

ProgressBar::ProgressBar() {
    this->done = false;

    this->min = DEFAULT_MIN;
    this->max = DEFAULT_MAX;
    this->progress = this->min;
    
    this->left = DEFAULT_LEFT;
    this->right = DEFAULT_RIGHT;

    this->width = DEFAULT_WIDTH;

    this->last_draw = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    this->interval = DEFAULT_INTERVAL;

    // Fetch the width of the command line if needed
    if (this->width < 0) {
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

        this->width = size.ws_col;
    }

    // Compute the total width of the bar section. This is defined as the width - 7 (for percentage and spaces on either side), - left.length() - right.length().
    this->bar_width = this->width - 8 - this->left.length() - this->right.length();
    // The inner width is the same minus to, to account for []
    this->bar_inner_width = this->bar_width - 2;
    // Check if this is possible
    if (this->bar_inner_width < 1) {
        throw runtime_error("Given width " + to_string(this->width) + " is too small for the progress bar.");
    }
}
ProgressBar::ProgressBar(long min_value, long max_value) {
    this->done = false;

    this->min = min_value;
    this->max = max_value;
    this->progress = this->min;
    
    this->left = DEFAULT_LEFT;
    this->right = DEFAULT_RIGHT;

    this->width = DEFAULT_WIDTH;

    this->last_draw = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    this->interval = DEFAULT_INTERVAL;

    // Fetch the width of the command line if needed
    if (this->width < 0) {
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

        this->width = size.ws_col;
    }

    // Compute the total width of the bar section. This is defined as the width - 7 (for percentage and spaces on either side), - left.length() - right.length().
    this->bar_width = this->width - 8 - this->left.length() - this->right.length();
    // The inner width is the same minus to, to account for []
    this->bar_inner_width = this->bar_width - 2;
    // Check if this is possible
    if (this->bar_inner_width < 1) {
        throw runtime_error("Given width " + to_string(this->width) + " is too small for the progress bar.");
    }
}
ProgressBar::ProgressBar(long min_value, long max_value, std::string left_text, std::string right_text) {
    this->done = false;

    this->min = min_value;
    this->max = max_value;
    this->progress = this->min;
    
    this->left = left_text;
    this->right = right_text;

    this->width = DEFAULT_WIDTH;

    this->last_draw = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    this->interval = DEFAULT_INTERVAL;

    // Fetch the width of the command line if needed
    if (this->width < 0) {
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

        this->width = size.ws_col;
    }

    // Compute the total width of the bar section. This is defined as the width - 7 (for percentage and spaces on either side), - left.length() - right.length().
    this->bar_width = this->width - 8 - this->left.length() - this->right.length();
    // The inner width is the same minus to, to account for []
    this->bar_inner_width = this->bar_width - 2;
    // Check if this is possible
    if (this->bar_inner_width < 1) {
        throw runtime_error("Given width " + to_string(this->width) + " is too small for the progress bar.");
    }
}
ProgressBar::ProgressBar(long min_value, long max_value, std::string left_text, std::string right_text, int width) {
    this->done = false;

    this->min = min_value;
    this->max = max_value;
    this->progress = this->min;
    
    this->left = left_text;
    this->right = right_text;

    this->width = width;

    this->last_draw = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    this->interval = DEFAULT_INTERVAL;

    // Fetch the width of the command line if needed
    if (this->width < 0) {
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

        this->width = size.ws_col;
    }

    // Compute the total width of the bar section. This is defined as the width - 7 (for percentage and spaces on either side), - left.length() - right.length().
    this->bar_width = this->width - 8 - this->left.length() - this->right.length();
    // The inner width is the same minus to, to account for []
    this->bar_inner_width = this->bar_width - 2;
    // Check if this is possible
    if (this->bar_inner_width < 1) {
        throw runtime_error("Given width " + to_string(this->width) + " is too small for the progress bar.");
    }
}
ProgressBar::ProgressBar(long min_value, long max_value, std::string left_text, std::string right_text, int width, chrono::milliseconds update_interval) {
    this->done = false;

    this->min = min_value;
    this->max = max_value;
    this->progress = this->min;
    
    this->left = left_text;
    this->right = right_text;

    this->width = width;

    this->last_draw = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    this->interval = update_interval;

    // Fetch the width of the command line if needed
    if (this->width < 0) {
        struct winsize size;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

        this->width = size.ws_col;
    }

    // Compute the total width of the bar section. This is defined as the width - 7 (for percentage and spaces on either side), - left.length() - right.length().
    this->bar_width = this->width - 8 - this->left.length() - this->right.length();
    // The inner width is the same minus to, to account for []
    this->bar_inner_width = this->bar_width - 2;
    // Check if this is possible
    if (this->bar_inner_width < 1) {
        throw runtime_error("Given width " + to_string(this->width) + " is too small for the progress bar.");
    }
}

void ProgressBar::draw() const {
    cout << '\r';
    cout << this->left;
    cout << " [";

    // Draw as many '=' as we have progress
    double ratio = ((double) (this->progress - this->min) / (double) (this->max) - this->min);
    int things_to_print = ratio * this->bar_inner_width;

    for (int i = 0; i < things_to_print; i++) {
        cout << '=';
    }
    for (int i = things_to_print; i < this->bar_inner_width; i++) {
        cout << ' ';
    }

    cout << "] ";
    // Draw percentage
    cout << ((int) (ratio * 100)) << "%";

    cout.flush();
}

void ProgressBar::win() {
    // Make sure progress isn't too large
    if (this->progress > this->max) {
        this->progress = this->max;
    }
    // Do a draw
    this->draw();
    // Print endline to finish it up
    cout << endl;
    this->done = true;
}

void ProgressBar::update(long amount) {
    if (this->done) {
        return;
    }

    // Update the progress
    this->progress += amount;
    
    // Check if we are done
    if (this->progress >= this->max) {
        return this->win();
    }

    // Otherwise, check for draw call
    chrono::milliseconds now = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch());
    if (now - this->last_draw >= this->interval) {
        // Do the draw call
        this->draw();

        // Update the clock
        this->last_draw += this->interval;

        // Reset to now if we're an entire interval or more behind
        if (now - this->last_draw >= this->interval) {
            this->last_draw = now;
        }
    }
}
