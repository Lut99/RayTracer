/* TEST PROGRESSBAR.cpp
 *   by Lut99
 *
 * Created:
 *   1/22/2020, 5:24:14 PM
 * Last edited:
 *   1/22/2020, 5:33:33 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file tests ProgressBar.cpp.
**/

#include <chrono>
#include <iostream>
#include <thread>

#include "../../src/lib/include/ProgressBar.hpp"

using namespace std;


bool test_bar() {
    cout << "Testing the bar..." << endl;
    
    cout << "  Creating class..." << endl;
    ProgressBar prgrs(0, 9999);
    cout << "  Looping and writing..." << endl;
    for (int i = 0; i < 10000; i++) {
        prgrs.update();

        std::this_thread::sleep_for(chrono::milliseconds(1));
    }
    
    cout << "Success" << endl << endl;
}


int main() {
    cout << "*** Test file for \"ProgressBar.cpp\" ***" << endl << endl;

    if (!test_bar()) {
        return 1;
    }
    return 0;
}
