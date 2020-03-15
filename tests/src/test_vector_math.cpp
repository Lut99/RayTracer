/* TEST VECTOR MATH.cpp
 *   by Lut99
 *
 * Created:
 *   2/2/2020, 6:25:46 PM
 * Last edited:
 *   3/3/2020, 4:55:00 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file is there to visualise and test how Vec3 objects do math. It
 *   outputs to Images using the Image class.
**/

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "../../src/lib/include/Vec3.hpp"
#include "../../src/lib/include/Image.hpp"

#if defined MACOS || defined _WIN32
#define sqrtf64 sqrt
#define sinf64 sin
#define cosf64 cos
#define acosf64 acos
#endif

using namespace std;
using namespace RayTracer;


void write_vec3_to_img(Vec3& vec, Image& out, double width=1) {
    // Draw a point with width=1 to the image at the vector location
    for (int y = out.height - 1; y >= 0; y--) {
        for (int x = 0; x < out.width; x++) {
            int dx = abs(x - (vec.x + 250));
            int dy = abs(y - (-vec.z + 250));

            if (sqrtf64(dx * dx + dy * dy) <= width) {
                out[y][x] = Vec3(0.0, 0.0, 0.0);
            }
        }
    }
}


int main() {
    Image out(500, 500);

    // Fill it with white
    for (int y = out.height - 1; y >= 0; y--) {
        for (int x = 0; x < out.width; x++) {
            out[y][x] = Vec3(1.0, 1.0, 1.0);
        }
    }

    double radius = 50;
    Vec3 c(50, 0, 0);
    Vec3 u(c.x, c.y, c.z + radius);
    Vec3 p(c.x + radius, c.y, c.z);
    
    // Make a circle
    Vec3 tu, tp;
    tu = (u - c).normalize();
    tp = (p - c).normalize();
    double start_angle = acosf64(dot(tu, tp));
    static double speed = 2 * M_PI / 500;
    double angle = start_angle;
    for (double k = 0; k < 2000; k++) {
        Vec3 p2 = Vec3(
            c.x + sinf64(angle) * radius,
            0,
            c.z + cosf64(angle) * radius
        );

        if (k == 0) {
            write_vec3_to_img(p2, out, 5);
        } else {
            write_vec3_to_img(p2, out, 2);
        }

        angle += speed;
    }

    out.to_png("tests/bin/out.png");
}