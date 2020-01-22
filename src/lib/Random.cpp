/* RANDOM.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 3:20:02 PM
 * Last edited:
 *   1/22/2020, 3:21:34 PM
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

#include "include/Random.hpp"

double RayTracer::random_double() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> rand_generator = std::bind(distribution, generator);
    return rand_generator();
}