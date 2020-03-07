# Add them to be linked to the raytracer executable
add_library(CameraRotation ${CMAKE_SOURCE_DIR}/src/lib/animations/camera/CameraRotation.cpp)

# Add them to be linked to the raytracer executable
list(APPEND EXTRA_LIBS CameraRotation)

# Finally, add the include folder for all these
list(APPEND EXTRA_INCLUDES "${CMAKE_SOURCE_DIR}/src/lib/include/animations/camera")