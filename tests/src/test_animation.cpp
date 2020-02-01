/* TEST ANIMATION.cpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 1:24:02 PM
 * Last edited:
 *   2/1/2020, 2:01:57 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file tests Animation.cpp. Primarily, it fills a set of 10 frames
 *   with a slowly moving object, and then outputs these to pictures.
**/

#include <thread>
#include <iostream>

#include "../../src/lib/include/Animation.hpp"

using namespace std;
using namespace RayTracer;


bool test_animation() {
    cout << "Testing the animation class..." << endl;

    cout << "  Creating class with 10 frames..." << endl;
    Animation ani(200, 100, 10, 1, "tests/bin/test_animation1", false);

    cout << "  Rendering simple, moving square..." << endl;
    for (int i = 0; i < ani.n_frames; i++) {
        for (int y = ani.height - 1; y >= 0; y--) {
            for (int x = 0; x < ani.width; x++) {
                ani[y][x] = Vec3(0, 0, 0);
                if (y >= 40 && y < 60 && x >= 40 + i * 10 && x < 60 + i * 10) {
                    ani[y][x] = Vec3(1, 1, 1);
                }
            }
        }
        ani.next();
    }

    cout << "  Writing output images..." << endl;
    ani.to_mp4("tests/bin/test_animation1/test.mp4");

    cout << "Success" << endl << endl;
    return true;
}

bool test_dynamic() {
    cout << "Testing the animation class with dynamic saving..." << endl;

    cout << "  Creating class with 10 frames..." << endl;
    Animation ani(200, 100, 10, 1, "tests/bin/test_animation2", true);

    cout << "  Rendering simple, moving square..." << endl;
    for (int i = 0; i < ani.n_frames; i++) {
        for (int y = ani.height - 1; y >= 0; y--) {
            for (int x = 0; x < ani.width; x++) {
                ani[y][x] = Vec3(0, 0, 0);
                if (y >= 40 && y < 60 && x >= 40 + i * 10 && x < 60 + i * 10) {
                    ani[y][x] = Vec3(1, 1, 1);
                }
            }
        }
        ani.next();

        // Wait a bit to make the spawning process visible
        this_thread::sleep_for(chrono::milliseconds(500));
    }

    cout << "  Creating mp4..." << endl;
    ani.to_mp4("tests/bin/test_animation2/test.mp4");

    cout << "Success" << endl << endl;
    return true;
}

int main() {
    cout << endl << "*** TEST FOR \"Animation.cpp\" ***" << endl << endl;
    if (!test_animation()) {
        return -1;
    }
    if (!test_dynamic()) {
        return -1;
    }
}