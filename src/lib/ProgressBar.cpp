/* PROGRESS BAR.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 4:44:56 PM
 * Last edited:
 *   05/03/2020, 18:14:31
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The ProgessBar file specializes at showing neat progress bars, and is
 *   usable in about any project. Is derived from Lut99's Python
 *   ProgressBar implementation. This file is the implementation file for
 *   ProgressBar.hpp.
**/

#ifndef _WIN32
#include <sys/ioctl.h>
#include <unistd.h>
#else
#include <windows.h>
#endif

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
    this->min = DEFAULT_MIN;
    this->max = DEFAULT_MAX;
    
    this->left = DEFAULT_LEFT;
    this->right = DEFAULT_RIGHT;

    this->width = DEFAULT_WIDTH;
    this->interval = DEFAULT_INTERVAL;

    this->init();
}
ProgressBar::ProgressBar(long min_value, long max_value) {
    this->min = min_value;
    this->max = max_value;
    
    this->left = DEFAULT_LEFT;
    this->right = DEFAULT_RIGHT;

    this->width = DEFAULT_WIDTH;
    this->interval = DEFAULT_INTERVAL;

    this->init();
}
ProgressBar::ProgressBar(long min_value, long max_value, std::string left_text, std::string right_text) {
    this->min = min_value;
    this->max = max_value;
    
    this->left = left_text;
    this->right = right_text;

    this->width = DEFAULT_WIDTH;
    this->interval = DEFAULT_INTERVAL;

    this->init();
}
ProgressBar::ProgressBar(long min_value, long max_value, std::string left_text, std::string right_text, int width) {
    this->min = min_value;
    this->max = max_value;
    
    this->left = left_text;
    this->right = right_text;

    this->width = width;
    this->interval = DEFAULT_INTERVAL;

    this->init();
}
ProgressBar::ProgressBar(long min_value, long max_value, std::string left_text, std::string right_text, int width, chrono::milliseconds update_interval) {
    this->min = min_value;
    this->max = max_value;
    
    this->left = left_text;
    this->right = right_text;

    this->width = width;
    this->interval = update_interval;

    this->init();
}

void ProgressBar::init() {
    this->var_width = this->width < 0;
    this->progress = this->min;
    this->done = false;

    this->last_draw = chrono::duration_cast<chrono::milliseconds>(chrono::system_clock::now().time_since_epoch()) - this->interval;
}

void ProgressBar::draw() const {
    // If needed, first get the width of the console
    size_t width = this->width;
    if (this->var_width) {
        width = this->resize_to_term_width();
    }
    // The inner width is the same minus to, to account for []
    size_t bar_inner_width = width - 8 - this->left.length() - this->right.length() - 2;
    // Check if this is possible
    if (bar_inner_width < 1) {
        throw runtime_error("Given width " + to_string(width) + " is too small for the progress bar.");
    }

    cout << '\r';
    cout << this->left;
    cout << " [";

    // Draw as many '=' as we have progress
    double ratio = ((double) (this->progress - this->min) / (double) (this->max) - this->min);
    int things_to_print = (int) (ratio * bar_inner_width);

    for (int i = 0; i < things_to_print; i++) {
        cout << '=';
    }
    for (int i = things_to_print; i < bar_inner_width; i++) {
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

int ProgressBar::resize_to_term_width() const {
    #ifndef _WIN32

    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    return size.ws_col;

    #else

    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;

    #endif
}

void ProgressBar::update(long amount) {
    this->set(this->progress + amount);
}

void ProgressBar::set(long value) {
    if (this->done) {
        return;
    }

    // Update the progress
    this->progress = value;
    
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
