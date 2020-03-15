/* VEC 3.hpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 12:11:32 PM
 * Last edited:
 *   2/4/2020, 4:22:24 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Vec3 class is a custom-written vector class. It holds three
 *   values: x, y and z, and contains several useful math functions for
 *   vectors in rendering. This specific file is the header file for
 *   Vec3.cpp.
**/

#ifndef VEC3_HPP
#define VEC3_HPP

#include <math.h>
#include "json.hpp"

namespace RayTracer {
    class Vec3 {
        public:
            /* The x value (first) in the vector. */
            double x;
            /* The y value (second) in the vector. */
            double y;
            /* The z value (third) in the vector. */
            double z;

            /* The Vec3 class provides math for a three-dimensional positional vector. The default version is initialized to (0,0,0). */
            Vec3();
            /* The Vec3 class provides math for a three-dimensional positional vector. This version is initialized with the given x, y and z. */
            Vec3(double x, double y, double z);

            /* Copy operator for the Vec3 class. */
            Vec3& operator=(const Vec3& other);

            /* Positive sign operator for the Vec3 class. */
            Vec3 operator+() const;
            /* Changing addition operator for the Vec3 class. */
            Vec3& operator+=(const Vec3& other);
            /* Changing addition operator for the Vec3 class, but then with a double. */
            Vec3& operator+=(const double other);

            /* Negative sign operator for the Vec3 class. */
            Vec3 operator-() const;
            /* Changing subtraction operator for the Vec3 class. */
            Vec3& operator-=(const Vec3& other);
            /* Changing subtraction operator for the Vec3 class, but then with a double. */
            Vec3& operator-=(const double other);

            /* Changing multiplication operator for the Vec3 class. */
            Vec3& operator*=(const Vec3& other);
            /* Changing multiplication operator for the Vec3 class, but then with a double. */
            Vec3& operator*=(const double other);

            /* Changing dividation operator for the Vec3 class. */
            Vec3& operator/=(const Vec3& other);
            /* Changing dividation operator for the Vec3 class, but then with a double. */
            Vec3& operator/=(const double other);

            /* Provides array-like access to the three vector values. */
            double& operator[](const int index);
            /* Constant, integer-based access to the vector values. */
            double get(const int index) const;

            /* Returns the length of the vector. */
            double length() const;
            /* Returns the length squared of the vector. */
            double squared_length() const;

            /* Returns the normalized version of this vector. */
            Vec3 normalize() const;

            /* Returns a json object describing this Vec3 object. */
            virtual nlohmann::json to_json() const;
            /* Returns a fresh Vec3 object as described in given json object. */
            static Vec3 from_json(nlohmann::json json_obj);
    };

    /* Constant addition operator for the Vec3 class (Vec3 + Vec3). */
    Vec3 operator+(const Vec3& v1, const Vec3& v2);
    /* Constant addition operator for the Vec3 class (Vec3 + double). */
    Vec3 operator+(const Vec3& v1, const double n);
    /* Constant addition operator for the Vec3 class (double + Vec3). */
    Vec3 operator+(const double n, const Vec3& v2);

    /* Constant addition operator for the Vec3 class (Vec3 - Vec3). */
    Vec3 operator-(const Vec3& v1, const Vec3& v2);
    /* Constant addition operator for the Vec3 class (Vec3 - double). */
    Vec3 operator-(const Vec3& v1, const double n);
    /* Constant addition operator for the Vec3 class (double - Vec3). */
    Vec3 operator-(const double n, const Vec3& v2);

    /* Constant addition operator for the Vec3 class (Vec3 * Vec3). */
    Vec3 operator*(const Vec3& v1, const Vec3& v2);
    /* Constant addition operator for the Vec3 class (Vec3 * double). */
    Vec3 operator*(const Vec3& v1, const double n);
    /* Constant addition operator for the Vec3 class (double * Vec3). */
    Vec3 operator*(const double n, const Vec3& v2);

    /* Constant addition operator for the Vec3 class (Vec3 / Vec3). */
    Vec3 operator/(const Vec3& v1, const Vec3& v2);
    /* Constant addition operator for the Vec3 class (Vec3 / double). */
    Vec3 operator/(const Vec3& v1, const double n);
    /* Constant addition operator for the Vec3 class (double / Vec3). */
    Vec3 operator/(const double n, const Vec3& v2);

    /* Returns the dot product of two vectors. */
    double dot(const Vec3& v1, const Vec3& v2);
    /* Returns the cross product of two vectors. */
    Vec3 cross(const Vec3& v1, const Vec3& v2);
}

#endif