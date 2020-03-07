# Add the libraries
add_library(RenderObjectCollection ${CMAKE_SOURCE_DIR}/src/lib/objects/RenderObjectCollection.cpp)
add_library(Sphere ${CMAKE_SOURCE_DIR}/src/lib/objects/Sphere.cpp)

# Add them to be linked to the raytracer executable
list(APPEND EXTRA_LIBS RenderObjectCollection)
list(APPEND EXTRA_LIBS Sphere)

# Finally, add the include folder for all these
list(APPEND EXTRA_INCLUDES "${CMAKE_SOURCE_DIR}/src/lib/include/objects")