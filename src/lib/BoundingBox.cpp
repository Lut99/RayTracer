/* BOUNDING BOX.cpp
 *   by Lut99
 *
 * Created:
 *   3/20/2020, 11:59:34 AM
 * Last edited:
 *   11/04/2020, 18:12:56
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The BoundingBox class is a simple wrapper for the BoundingBoxes used
 *   in the optimised tree search for objects. It does not much more than
 *   to store the two points that make up the box and one function to merge
 *   it with a larger one, and one function to check if it has been hit by
 *   a ray. This file is the implementation file for BoundingBox.hpp.
**/

#include <cmath>
#include <iostream>

#include "BoundingBox.hpp"

#if defined MACOS || defined _WIN32
#define fabsf64 fabs
#endif

using namespace std;
using namespace RayTracer;


bool BoundingBox::hit(const Ray& ray, double t_min, double t_max) const {
    // Fetch the correct order of the hits
    for (int i = 0; i < 3; i++) {
        // Compute the t's
        double invD = 1.0 / ray.direction.get(i);
        double t0 = (this->p1.get(i) - ray.origin.get(i)) * invD;
        double t1 = (this->p2.get(i) - ray.origin.get(i)) * invD;
        if (invD < 0.0) {
            std::swap(t0, t1);
        }

        // Mix in the given min and max t values
        t_min = t0 > t_min ? t0 : t_min;
        t_max = t1 < t_max ? t1 : t_max;

        cout << "t_min: " << t_min << endl;
        cout << "t_max: " << t_max << endl;

        if (t_max <= t_min) {
            return false;
        }
    }
    cout << "Hitbox returns true!" << endl;
    return true;
}

void BoundingBox::merge(const BoundingBox& other) {
    // For each of the three dimensions, find the smallest and put that in
    //   this->p1. The larger is put in this->p2.
    for (int i = 0; i < 3; i++) {
        double a1 = this->p1.get(i);
        double a2 = this->p2.get(i);
        double b1 = other.p1.get(i);
        double b2 = other.p2.get(i);

        // Order the A-box values
        double min = a1;
        double max = a2;
        if (min > max) {
            min = max;
            max = a1;
        }

        // Add the B-box values
        if (b1 < min) {
            min = b1;
        } else if (b1 > max) {
            max = b1;
        }
        if (b2 < min) {
            min = b2;
        } else if (b2 > max) {
            max = b2;
        }

        // Set the values and return
        this->p1[i] = min;
        this->p2[i] = max;
    }
}

double BoundingBox::size() const {
    double dx = fabsf64(this->p1.x - this->p2.x);
    double dy = fabsf64(this->p1.y - this->p2.y);
    double dz = fabsf64(this->p1.z - this->p2.z);
    return dx * dy * dz;
}
