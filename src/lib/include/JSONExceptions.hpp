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

namespace RayTracer {
    class JSONException: public std::exception {
        public:
            const std::string object_name;

            /* Base exception for all JSON-related exceptions. */
            JSONException(const std::string object)
                : object_name(object)
            {}

            /* Returns a human-readable string of what is wrong */
            virtual const char* what() const noexcept {
                return (this->object_name + ": An unknown exception occured").c_str();
            }
    };
    class InvalidTypeException: public JSONException {
        public:
            const std::string expected;
            const std::string got;

            /* Exception for when an unexpected type is encountered. */
            InvalidTypeException(const std::string object, const std::string expected_type, const std::string got_type)
                : JSONException(object),
                expected(expected_type),
                got(got_type)
            {}

            /* Returns a human-readable string of what is wrong */
            virtual const char* what() const noexcept {
                return (this->object_name + ": Expected type " + this->expected + ", but got type " + this->got).c_str();
            }
    };
    class InvalidFieldFormat: public InvalidTypeException {
        public:
            const std::string field_name;
            const std::string expected;
            const std::string got;

            /* Special case of a type exception that works on a field of given object or array. */
            InvalidFieldFormat(const std::string object, const std::string field, const std::string expected_type, const std::string got_type)
                : InvalidTypeException(object, expected_type, got_type),
                field_name(field)
            {}

            /* Returns a human-readable string of what is wrong */
            virtual const char* what() const noexcept {
                return (this->object_name + ": Expected field '" + this->field_name + "' to be of type " + this->expected + ", but got type " + this->got).c_str();
            }
    };
    class InvalidLengthException: public JSONException {
        public:
            const std::size_t expected;
            const std::size_t got;

            /* Exception for when an array or object has an unexpected length. */
            InvalidLengthException(const std::string object, const std::size_t expected_size, const std::size_t got_size)
                : JSONException(object),
                expected(expected_size),
                got(got_size)
            {}

            /* Returns a human-readable string of what is wrong */
            virtual const char* what() const noexcept {
                return (this->object_name + ": Expected array or object to have size " + std::to_string(this->expected) + ", but got size " + std::to_string(this->got)).c_str();
            }
    };
    class MissingFieldException: public JSONException {
        public:
            const std::string missing_field;

            /* Exception for when a field was expected but is not there */
            MissingFieldException(const std::string object, const std::string field_name)
                : JSONException(object),
                missing_field(field_name)
            {}

            /* Returns a human-readable string of what is wrong */
            virtual const char* what() const noexcept {
                return (this->object_name + ": Expected field '" + this->missing_field + "'").c_str();
            }
    };
    class UnknownSubtypeException: public JSONException {
        public:
            const unsigned long unknown_type;

            /* Exception for when a type had to be parsed, but that type was unknown. */
            UnknownSubtypeException(const std::string object, const unsigned long type)
                : JSONException(object),
                unknown_type(type)
            {}

            /* Returns a human-readable string of what is wrong */
            virtual const char* what() const noexcept {
                return (this->object_name + ": Unknown subtype '" + std::to_string(this->unknown_type) + "'").c_str();
            }
    };
}

#endif