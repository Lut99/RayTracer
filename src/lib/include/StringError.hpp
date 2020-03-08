/* STRING ERROR.hpp
 *   by Lut99
 *
 * Created:
 *   08/03/2020, 15:26:03
 * Last edited:
 *   08/03/2020, 15:26:03
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file wraps the C11 strerror_s and strerrorlen_s functions. On
 *   Windows, defines the latter as well.
**/

#ifndef STRINGERROR_HPP
#define STRINGERROR_HPP

#include <cerrno>
#include <string>


namespace std {
    /* C++ wrapper for strerror_s and the matching strerrorlen_s. */
    std::string string_error(errno_t errnum);
}

#endif