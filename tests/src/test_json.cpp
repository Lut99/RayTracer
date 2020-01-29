/* TEST JSON.cpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 5:43:27 PM
 * Last edited:
 *   1/29/2020, 6:49:42 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file is meant to test how the JSON library works.
**/

#include <iostream>

#include "../../src/lib/include/json.hpp"

#include "../../src/lib/include/RenderObject.hpp"

using namespace std;
using namespace nlohmann;
using namespace RayTracer;

int main() {
    RenderObject o((1.0, 5.0, 3.0), sphere);
    json j = v.to_json();
    cout << j << endl;

    Vec3 v2(j);
    cout << "Vec3(" << v2.x << ", " << v2.y << ", " << v2.z << ")" << endl;
}