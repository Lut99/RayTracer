/* VEC 3.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 12:12:44 PM
 * Last edited:
 *   3/3/2020, 4:44:35 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The Vec3 class is a custom-written vector class. It holds three
 *   values: x, y and z, and contains several useful math functions for 
 *   vectors in rendering. This particular file is the implementaition file
 *   for Vec3.hpp.
**/

#include <stdexcept>

#include "include/JSONExceptions.hpp"
#include "include/Vec3.hpp"

#ifdef MACOS
#define sqrtf64 sqrt
#endif

using namespace std;
using namespace RayTracer;
using namespace nlohmann;


Vec3::Vec3() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}
Vec3::Vec3(double x, double y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3& Vec3::operator=(const Vec3& other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    return *this;
}

Vec3 Vec3::operator+() const {
    // Simply return a copy of this
    return *this;
}
Vec3& Vec3::operator+=(const Vec3& other) {
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;

    return *this;
}
Vec3& Vec3::operator+=(const double other) {
    this->x += other;
    this->y += other;
    this->z += other;

    return *this;
}

Vec3 Vec3::operator-() const {
    return Vec3(-this->x, -this->y, -this->z);
}
Vec3& Vec3::operator-=(const Vec3& other) {
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;

    return *this;
}
Vec3& Vec3::operator-=(const double other) {
    this->x -= other;
    this->y -= other;
    this->z -= other;

    return *this;
}

Vec3& Vec3::operator*=(const Vec3& other) {
    this->x *= other.x;
    this->y *= other.y;
    this->z *= other.z;

    return *this;
}
Vec3& Vec3::operator*=(const double other) {
    this->x *= other;
    this->y *= other;
    this->z *= other;

    return *this;
}

Vec3& Vec3::operator/=(const Vec3& other) {
    this->x /= other.x;
    this->y /= other.y;
    this->z /= other.z;

    return *this;
}
Vec3& Vec3::operator/=(const double other) {
    this->x /= other;
    this->y /= other;
    this->z /= other;

    return *this;
}

double& Vec3::operator[](int index) {
    if (index == 0) {
        return this->x;
    } else if (index == 1) {
        return this->y;
    } else if (index == 2) {
        return this->z;
    }
    throw out_of_range("Index " + to_string(index) + " is out of range for Vec3");
}

double Vec3::length() const {
    return sqrtf64(this->x * this->x + this->y * this->y + this->z * this->z);
}
double Vec3::squared_length() const {
    double l = this->length();
    return l * l;
}

Vec3 Vec3::normalize() const {
    double l = this->length();
    return Vec3(
        this->x / l,
        this->y / l,
        this->z / l
    );
}

json Vec3::to_json() const {
    json j = json::array();
    j[0] = this->x;
    j[1] = this->y;
    j[2] = this->z;
    return j;
}
Vec3 Vec3::from_json(json json_obj) {
    // First, check if the json_obj is an array
    if (!json_obj.is_array()) {
        throw InvalidTypeException("Vec3", json::array().type_name(), json_obj.type_name());
    }

    // Next, check for the array length
    if (json_obj.size() != 3) {
        throw InvalidLengthException("Vec3", 3, json_obj.size());
    }

    // Then, try to parse it as three doubles
    Vec3 to_return;
    try {
        to_return.x = json_obj[0].get<double>();
        to_return.y = json_obj[1].get<double>();
        to_return.z = json_obj[2].get<double>();
    } catch (nlohmann::detail::type_error& e) {
        throw InvalidTypeException("Vec3", "double", json_obj[0].type_name());
    }
    return to_return;
}



Vec3 RayTracer::operator+(const Vec3& v1, const Vec3& v2) {
    return Vec3(
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z
    );
}
Vec3 RayTracer::operator+(const Vec3& v1, const double n) {
    return Vec3(
        v1.x + n,
        v1.y + n,
        v1.z + n
    );
}
Vec3 RayTracer::operator+(const double n, const Vec3& v2) {
    return Vec3(
        n + v2.x,
        n + v2.y,
        n + v2.z
    );
}

Vec3 RayTracer::operator-(const Vec3& v1, const Vec3& v2) {
    return Vec3(
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z
    );
}
Vec3 RayTracer::operator-(const Vec3& v1, const double n) {
    return Vec3(
        v1.x - n,
        v1.y - n,
        v1.z - n
    );
}
Vec3 RayTracer::operator-(const double n, const Vec3& v2) {
    return Vec3(
        n - v2.x,
        n - v2.y,
        n - v2.z
    );
}

Vec3 RayTracer::operator*(const Vec3& v1, const Vec3& v2) {
    return Vec3(
        v1.x * v2.x,
        v1.y * v2.y,
        v1.z * v2.z
    );
}
Vec3 RayTracer::operator*(const Vec3& v1, const double n) {
    return Vec3(
        v1.x * n,
        v1.y * n,
        v1.z * n
    );
}
Vec3 RayTracer::operator*(const double n, const Vec3& v2) {
    return Vec3(
        n * v2.x,
        n * v2.y,
        n * v2.z
    );
}

Vec3 RayTracer::operator/(const Vec3& v1, const Vec3& v2) {
    return Vec3(
        v1.x / v2.x,
        v1.y / v2.y,
        v1.z / v2.z
    );
}
Vec3 RayTracer::operator/(const Vec3& v1, const double n) {
    return Vec3(
        v1.x / n,
        v1.y / n,
        v1.z / n
    );
}
Vec3 RayTracer::operator/(const double n, const Vec3& v2) {
    return Vec3(
        n / v2.x,
        n / v2.y,
        n / v2.z
    );
}

double RayTracer::dot(const Vec3& v1, const Vec3& v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
Vec3 RayTracer::cross(const Vec3& v1, const Vec3& v2) {
    return Vec3(
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}
