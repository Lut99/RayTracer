/* CAMERA ROTATION.hpp
 *   by Lut99
 *
 * Created:
 *   2/1/2020, 4:56:07 PM
 * Last edited:
 *   2/8/2020, 11:14:38 PM
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

#include "../CameraMovement.hpp"

namespace RayTracer {
    class CameraRotation: public CameraMovement {
        private:
            Vec3 center;
            double radius;
            double angle;

            /* This function computes the distance on a circle the camera has to travel each frame for the circle_time to be made */
            static double compute_speed(std::chrono::seconds circle_time);
        public:
            const std::chrono::seconds loop_time;
            const double speed;

            /* The CameraRotation class describes how a Camera object rotates along a specified circle. The center of it is the Camera's lookat point, and the radius is given. The speed variable determines how many rotations per second the camera makes. */
            CameraRotation(Camera* target_cam, std::chrono::seconds circle_time);

            /* The update class updates the position of the camera along the circle. */
            virtual void update(std::chrono::milliseconds time_passed);
    };
}

#endif