/* RANDOM.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 3:20:21 PM
 * Last edited:
 *   1/29/2020, 11:44:18 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file harbours a simpel random function, that can generate random
 *   doubles within 0.0 to 1.0. This particular file is the header file for
 *   Random.cpp.
**/

#ifndef RANDOM_HPP
#define RANDOM_HPP

#include "Vec3.hpp"

namespace RayTracer {
    /* Generates a random double such that 0 <= x < 1 */
    double random_double();
    /* Generates a vector randomly in the unit sphere. Uses a rather ugly method that has undertermined runtime. */
    Vec3 random_in_unit_sphere();
    /* Generates a vector randomly in the unit disc. Basically random_in_unit_sphere() except in 2D. */
    Vec3 random_in_unit_disc();
}

#endif