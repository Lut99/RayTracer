# Add them to be linked to the raytracer executable
add_library(Dielectric ${CMAKE_SOURCE_DIR}/src/lib/materials/Dielectric.cpp)
add_library(Lambertian ${CMAKE_SOURCE_DIR}/src/lib/materials/Lambertian.cpp)
add_library(Metal ${CMAKE_SOURCE_DIR}/src/lib/materials/Metal.cpp)

# Add them to be linked to the raytracer executable
list(APPEND EXTRA_LIBS Dielectric)
list(APPEND EXTRA_LIBS Lambertian)
list(APPEND EXTRA_LIBS Metal)

# Finally, add the include folder for all these
list(APPEND EXTRA_INCLUDES "${CMAKE_SOURCE_DIR}/src/lib/include/materials")