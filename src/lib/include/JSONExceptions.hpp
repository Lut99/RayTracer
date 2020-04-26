/* JSONEXCEPTIONS.hpp
 *   by Anonymous
 *
 * Created:
 *   2/4/2020, 4:23:23 PM
 * Last edited:
 *   2/4/2020, 4:24:55 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   This file contains the errors as they appear for JSON parsing and
 *   loading.
**/

#ifndef JSONEXCEPTIONS_HPP
#define JSONEXCEPTIONS_HPP

#include <exception>
#include <string>
#include <cerrno>
#include <cstring>

namespace RayTracer {
    class JSONException: public std::exception {
        public:
            const std::string object_name;
            const std::string msg;

            /* Base exception for all JSON-related exceptions. */
            JSONException(const std::string object, const std::string message)
                : object_name(object),
                msg("ERROR: " + object + ": " + message)
            {}

            /* Returns a human-readable string of what is wrong */
            virtual const char* what() const throw() {
                return this->msg.c_str();
            }
    };



    class InvalidTypeException: public JSONException {
        public:
            const std::string expected;
            const std::string got;

            /* Exception for when an unexpected type is encountered. */
            InvalidTypeException(const std::string object, const std::string expected_type_name, const std::string got_type_name, const std::string message)
                : JSONException(object, message),
                expected(expected_type_name),
                got(got_type_name)
            {}
    };

    class InvalidObjectFormat: public InvalidTypeException {
        public:
            /* Special case of a type exception that works on a field of given object or array. */
            InvalidObjectFormat(const std::string object, const std::string expected_type_name, const std::string got_type_name)
                : InvalidTypeException(object, expected_type_name, got_type_name, "Expected root field to be of type " + expected_type_name + ", but is of type " + got_type_name)
            {}
    };

    class InvalidFieldFormat: public InvalidTypeException {
        public:
            const std::string field_name;

            /* Special case of a type exception that works on a field of given object or array. */
            InvalidFieldFormat(const std::string object, const std::string invalid_field_name, const std::string expected_type_name, const std::string got_type_name)
                : InvalidTypeException(object, expected_type_name, got_type_name, "Expected field '" + invalid_field_name + "' to be of type " + expected_type_name + ", but is of type " + got_type_name),
                field_name(invalid_field_name)
            {}
    };



    class InvalidLengthException: public JSONException {
        public:
            const std::size_t expected;
            const std::size_t got;

            /* Exception for when an array or object has an unexpected length. */
            InvalidLengthException(const std::string object, const std::size_t expected_size, const std::size_t got_size)
                : JSONException(object, "Expected array or object to have size " + std::to_string(expected_size) + ", but got size " + std::to_string(got_size)),
                expected(expected_size),
                got(got_size)
            {}
    };



    class MissingFieldException: public JSONException {
        public:
            const std::string field_name;

            /* Exception for when a field was expected but is not there */
            MissingFieldException(const std::string object, const std::string missing_field_name)
                : JSONException(object, "Expected field '" + missing_field_name + "'"),
                field_name(missing_field_name)
            {}
    };



    class UnknownSubtypeException: public JSONException {
        public:
            const unsigned long type;

            /* Exception for when a type had to be parsed, but that type was unknown. */
            UnknownSubtypeException(const std::string object, const unsigned long unknown_type)
                : JSONException(object, "Unknown subtype '" + std::to_string(unknown_type) + "'"),
                type(unknown_type)
            {}
    };



    class IOException: public JSONException {
        public:
            const std::string path;

            /* Exception for when a file needs to be accessed, but some kind of error occurred. */
            IOException(const std::string object, const std::string file_path, const std::string message)
                : JSONException(object, message),
                path(file_path)
            {}
    };

    class FileIOException: public IOException {
        public:
            const int errnum;

            /* Exception for when a file needs to be accessed, but some kind of error occurred. */
            FileIOException(const std::string object, const std::string file_path, const int error_number)
                : IOException(object, file_path, "An error occured while trying to open file '" + file_path + "': " + strerror(error_number)),
                errnum(error_number)
            {}
    };

    class SOLoadException: public IOException {
        public:
            const int errnum;

            /* Exception for when a dynamic link to a library could not be openend */
            SOLoadException(const std::string object, const std::string file_path, const int error_number)
                : IOException(object, file_path, "An error occurred loading shared object '" + file_path + "': " + strerror(error_number)),
                errnum(error_number)
            {}
    };

    class SOFuncLoadException: public SOLoadException {
        public:
            const std::string err_msg;
            const std::string func;

            /* Exception for when a dynamic link to a library could not be openend */
            SOFuncLoadException(const std::string object, const std::string file_path, const std::string func_name, const int error_number)
                : SOLoadException(object, file_path, error_number),
                err_msg("ERROR: " + this->object_name + ": An error occurred loading function '" + func_name+ "' from shared object '" + file_path + "': " + strerror(error_number)),
                func(func_name)
            {}

            virtual const char* what() const throw() {
                return this->err_msg.c_str();
            }
    };
    
    class SORunException: public IOException {
        public:
            const std::string func;
            const std::string err_msg;

            /* Exception for when any derived exception of std::exception is thrown during execution of a dynamically loaded function. */
            SORunException(const std::string object, const std::string file_path, const std::string func_name, const std::string message)
                : IOException(object, file_path, "An error occurred during execution of the function '" + func_name + "' in shared object '" + file_path + "': " + message),
                func(func_name),
                err_msg(message)
            {}
    };
}

#endif