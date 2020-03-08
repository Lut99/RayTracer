/* STRERRORLEN.cpp
 *   by Lut99
 *
 * Created:
 *   08/03/2020, 15:26:50
 * Last edited:
 *   08/03/2020, 15:26:50
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file wraps the C11 strerror_s and strerrorlen_s functions. On
 *   Windows, defines the latter as well.
**/

#include "StringError.hpp"

#include <cstring>

using namespace std;


#ifdef _WIN32
string std::string_error(errno_t errnum) {
    char errmsg[94];
    strerror_s(errmsg, 94, errnum);
    return string(errmsg);
}
#else
string std::string_error(int errnum) {
    return string(strerror(errnum));
}
#endif