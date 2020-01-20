/* TEST IMAGE.cpp
 *   by Lut99
 *
 * Created:
 *   1/20/2020, 3:39:04 PM
 * Last edited:
 *   1/20/2020, 5:33:33 PM
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

    cout << "  Creating image..." << endl;
    Image img = Image(200, 100);

    cout << "  Creating gradient..." << endl;
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 200; x++) {
            Pixel p = img[y][x];
            p.r() = float(x) / float(200);
            p.g() = float(y) / float(100);
            p.b() = 0.2;
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
