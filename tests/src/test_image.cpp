/* TEST IMAGE.cpp
 *   by Lut99
 *
 * Created:
 *   1/20/2020, 3:39:04 PM
 * Last edited:
 *   1/20/2020, 3:40:17 PM
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

#include "../../src/lib/include/Image.hpp"

using namespace std;
using namespace RayTracer;


bool test_values() {

}
bool test_ppm() {

}


int main() {
    if (!test_values()) {
        return 1;
    }

    return 0;
}
