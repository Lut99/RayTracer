/* TEST IMAGE.cpp
 *   by Lut99
 *
 * Created:
 *   1/20/2020, 3:39:04 PM
 * Last edited:
 *   1/22/2020, 12:09:33 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file tests Image.cpp. It mainly checks if the pixels that were
 *   put it can go out again, and if the resulting file is in the expected
 *   format.
**/

#include <iostream>
#include <fstream>
#include <sstream>

#include "../../src/lib/include/Image.hpp"


using namespace std;
using namespace RayTracer;


bool test_values() {
    cout << "Testing values of Image class..." << endl;

    cout << "  Creating array with values (50x50)..." << endl;
    double values[50][50][3];
    for (int y = 0; y < 50; y++) {
        for (int x = 0; x < 50; x++) {
            for (int z = 0; z < 3; z++) {
                values[y][x][z] = x / 50;
            }
        }
    }

    cout << "  Creating image (50x50)..." << endl;
    Image img = Image(50, 50);

    // Compare the values
    bool succes = true;
    int fx, fy, fz;
    for (int y = 0; y < 50; y++) {
        for (int x = 0; x < 50; x++) {
            for (int z = 0; z < 3; z++) {
                if (values[y][x][z] != img[y][x][z]) {
                    succes = false;
                    fx = x;
                    fy = y;
                    fz = z;
                    break;
                }
            }
            if (!succes) {break;}
        }
        if (!succes) {break;}
    }

    if (succes) {
        cout << "Succes" << endl << endl;
        return true;
    } else {
        cout << "Failure: expected " << values[fy][fx][fz] << " @ (" << fy << "," << fx << "," << fz << "), got " << img[fy][fx][fz] << endl << endl;
        return false;
    }
}
bool test_png() {
    cout << "Writing test image..." << endl;

    int w, h;
    w = 200;
    h = 100;

    cout << "  Creating image..." << endl;
    Image img = Image(w, h);

    cout << "  Creating gradient..." << endl;
    for (int y = h - 1; y >= 0; y--) {
        cout << y << endl;
        for (int x = 0; x < w; x++) {
            Pixel p = img[(h - 1) - y][x];
            p.r() = float(x) / float(w);
            p.g() = float(y) / float(h);
            p.b() = 0;
        }
    }

    cout << "  Writing..." << endl;
    img.to_png("test.png");

    cout << "Succes" << endl << endl;
}


int main() {
    cout << "*** Test file for \"Image.cpp\" ***" << endl << endl;

    if (!test_values()) {
        return 1;
    }
    if (!test_png()) {
        return 1;
    }

    return 0;
}
