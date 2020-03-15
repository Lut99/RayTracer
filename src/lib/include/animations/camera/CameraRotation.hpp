/* CAMERA ROTATION.hpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 4:56:07 PM
 * Last edited:
 *   07/03/2020, 11:48:17
 * Auto updated?
 *   Yes
 *
 * Description:
 *   The CameraRotation class models the movement of a Camera that circles
 *   around a point. In this animation, the Camera looks at the same point
 *   throughout the animation; just the lookfrom is changed. This
 *   particular file is the header file for CameraRotation.cpp.
**/

#ifndef CAMERAROTATION_HPP
#define CAMERAROTATION_HPP

#include "CameraMovement.hpp"

namespace RayTracer {
    class CameraRotation: public CameraMovement {
        private:
            Vec3 center;
            double radius;
            double angle;
            double speed;

            /* This function computes the distance on a circle the camera has to travel each frame for the circle_time to be made */
            static double compute_speed(std::chrono::seconds circle_time);
        public:
            const std::chrono::seconds loop_time;

            /* The CameraRotation class describes how a Camera object rotates along a specified circle. The center of it is the Camera's lookat point, and the radius is given. The speed variable determines how many rotations per second the camera makes. */
            CameraRotation(std::chrono::seconds circle_time);

            /* Virtual deconstructor for the CameraRotation class. */
            virtual ~CameraRotation() = default;

            /* Recomputes the start point and angle of the target Camera. */
            virtual void recompute(Camera* target);
            /* The update class updates the position of the camera along the circle. */
            virtual void update(std::chrono::milliseconds time_passed, Camera* target);

            /* Returns a json object based on this CameraRotation object. */
            virtual nlohmann::json to_json() const;
            /* Returns a new CameraRotation object from the given json object. Note that the returned object is allocated and has to be deallocated later on. */
            static CameraRotation* from_json(nlohmann::json json_obj);
    };
}

#endif