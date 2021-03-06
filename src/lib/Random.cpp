/* RANDOM.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 3:20:02 PM
 * Last edited:
 *   1/29/2020, 11:44:57 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file harbours a simpel random function, that can generate random
 *   doubles within 0.0 to 1.0. This particular file is the implementation
 *   file for Random.hpp.
**/

#include <random>
#include <functional>

#include "Random.hpp"

double RayTracer::random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> rand_generator = std::bind(distribution, generator);
    return rand_generator();
}

RayTracer::Vec3 RayTracer::random_in_unit_sphere() {
    Vec3 p;
    do {
        p = 2.0 * Vec3(random_double(), random_double(), random_double()) - 1.0;
    } while (p.squared_length() >= 1.0);
    return p;
}

RayTracer::Vec3 RayTracer::random_in_unit_disc() {
    Vec3 p;
    do {
        p = 2.0 * Vec3(random_double(), random_double(), 0) - Vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}