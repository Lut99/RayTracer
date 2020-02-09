/* CAMERA MOVEMENT.hpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 4:48:29 PM
 * Last edited:
 *   2/9/2020, 2:00:39 AM
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The CameraMovement class describes how the Camera can move around. It
 *   is in itself another animation baseclass, as there are multiple types
 *   of camera movements. This particular file is the header file for
 *   CameraMovement.cpp.
**/

#ifndef CAMERAMOVEMENT_HPP
#define CAMERAMOVEMENT_HPP

#include <chrono>

#include "json.hpp"
#include "Camera.hpp"

namespace RayTracer {
    class Camera;

    enum CameraMovementType {
        rotation
    };

    class CameraMovement {
        protected:
            /* The CameraMovement is a special type of animations that is applicaple on cameras. It is itself another baseclass for these animations. */
            CameraMovement(CameraMovementType movement_type);

            /* This function compiles CameraMovement-general properies to a given json object. */
            virtual void baseclass_to_json(nlohmann::json& json_obj) const;
        public:
            /* Describes the subtype of the movement. */
            const CameraMovementType type;

            /* Virtual deconstructor for the CameraMovement class. */
            virtual ~CameraMovement() = default;

            /* Recomputes the start point and angle of the target Camera. This might not be used by the derived class, but then they should simply implement a dummy function. */
            virtual void recompute(Camera* target) = 0;
            /* Virtual for the update() function. After each frame, this is called in the derived class to update the camera it animates. The number is the amount of milliseconds since the last frame in the movie (so not the rendertime). */
            virtual void update(std::chrono::milliseconds time_passed, Camera* target) = 0;

            /* Returns a json object based on this CameraMovement object. */
            virtual nlohmann::json to_json() const;
            /* Returns a child of the CameraMovement from a json object. Note that the returned object is allocated and has to be deallocated later on. */
            static CameraMovement* from_json(nlohmann::json json_obj);
    };
}

#endif