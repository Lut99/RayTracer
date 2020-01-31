/* WORLD IO.hpp
 *   by Lut99
 *
 * Created:
 *   1/29/2020, 7:41:56 PM
 * Last edited:
 *   1/31/2020, 1:15:04 PM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The WorldIO class can save and load RenderWorld objects to a JSON
 *   file. Note that it defines separate readers and writers for objects,
 *   materials and some other classes, so that they can be re-used if
 *   needed. This particular file is the header file for WorldIO.cpp.
**/

#ifndef WORLDIO_HPP
#define WORLDIO_HPP

#include <string>
#include <exception>

#include "json.hpp"
#include "RenderWorld.hpp"

#include "objects/Sphere.hpp"
#include "objects/RenderObjectCollection.hpp"

#include "materials/Lambertian.hpp"
#include "materials/Metal.hpp"
#include "materials/Dielectric.hpp"

namespace RayTracer {
    namespace WorldIO {

        /* EXCEPTIONS */

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


        /* PARSERS */

        /* Writes a RenderWorld object to a JSON object. */
        nlohmann::json to_json(const RenderWorld& world);
        /* Creates a new RenderWorld object from a JSON object. */
        RenderWorld* from_json(const nlohmann::json& json_obj);

        /* Writes a Camera object to a JSON object. */
        nlohmann::json to_json(const Camera& cam);
        /* Creates a new Camera object from a JSON object. */
        Camera* camera_from_json(const nlohmann::json& json_obj);

        /* Writes a Vec3 object to a JSON object. */
        nlohmann::json to_json(const Vec3& vec3);
        /* Creates a new Vec3 object from a JSON object. */
        Vec3 vec3_from_json(const nlohmann::json& json_obj);

        /* In the Objects namespace are the readers and writers for the RenderObject class and its derived classes. */
        namespace Objects {
            /* Writes a RenderObject object to a JSON object. */
            nlohmann::json to_json(const RenderObject& object);
            /* Creates a new RenderObject object from a JSON object. Note that the new object is allocated and will have to be deallocated. */
            RenderObject* from_json(const nlohmann::json& json_obj);

            /* Writes a Sphere object to a JSON object. */
            nlohmann::json to_json(const Sphere& object);
            /* Creates a new Sphere object from a JSON object. Note that the new object is allocated and will have to be deallocated. */
            Sphere* sphere_from_json(const nlohmann::json& json_obj);
            
            /* Writes a RenderObjectCollection object to a JSON object. */
            nlohmann::json to_json(const RenderObjectCollection& object);
            /* Creates a new RenderObjectCollection object from a JSON object. Note that the new object is allocated and will have to be deallocated. */
            RenderObjectCollection* renderobjectcollection_from_json(const nlohmann::json& json_obj);
        }

        /* In the Materials namespace are the readers and writers for the Material class and its derived classes. */
        namespace Materials {
            /* Writes a Material object to a JSON object. */
            nlohmann::json to_json(const Material& object);
            /* Creates a new Material object from a JSON object. Note that the new object is allocated and will have to be deallocated. */
            Material* from_json(const nlohmann::json& json_obj);

            /* Writes a Lambertian object to a JSON object. */
            nlohmann::json to_json(const Lambertian& object);
            /* Creates a new Lambertian object from a JSON object. Note that the new object is allocated and will have to be deallocated. */
            Lambertian* lambertian_from_json(const nlohmann::json& json_obj);

            /* Writes a Metal object to a JSON object. */
            nlohmann::json to_json(const Metal& object);
            /* Creates a new Metal object from a JSON object. Note that the new object is allocated and will have to be deallocated. */
            Metal* metal_from_json(const nlohmann::json& json_obj);

            /* Writes a Dielectric object to a JSON object. */
            nlohmann::json to_json(const Dielectric& object);
            /* Creates a new Dielectric object from a JSON object. Note that the new object is allocated and will have to be deallocated. */
            Dielectric* dielectric_from_json(const nlohmann::json& json_obj);
        }
    }
}

#endif