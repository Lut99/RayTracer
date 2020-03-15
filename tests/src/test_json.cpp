/* TEST JSON.cpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 5:43:27 PM
 * Last edited:
 *   1/29/2020, 8:18:33 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file is meant to test how the JSON library works.
**/

#include <iostream>

#include "../../src/lib/include/WorldIO.hpp"

using namespace std;
using namespace nlohmann;
using namespace RayTracer;

int main() {
    Vec3 v(1.01234123, 5.01235235, 3.0234523);
    json j = WorldIO::to_json(v);
    cout << j << endl;

    Vec3 v2 = WorldIO::vec3_from_json(j);
    cout << "Vec3(" << v2.x << ", " << v2.y << ", " << v2.z << ")" << endl;
}