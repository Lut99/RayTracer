# Add the libraries
add_library(RandomScene ${CMAKE_SOURCE_DIR}/src/lib/scenes/RandomScene.cpp)

# Add them to be linked to the raytracer executable
list(APPEND EXTRA_LIBS RandomScene)

# Finally, add the include folder for all these
list(APPEND EXTRA_INCLUDES "${CMAKE_SOURCE_DIR}/src/lib/include/scenes")