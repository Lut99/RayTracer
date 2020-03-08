# Include the libraries from subdirectories
include(${CMAKE_SOURCE_DIR}/src/lib/animations/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/src/lib/materials/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/src/lib/objects/CMakeLists.txt)
include(${CMAKE_SOURCE_DIR}/src/lib/scenes/CMakeLists.txt)

# Add the libraries
add_library(Camera ${CMAKE_SOURCE_DIR}/src/lib/Camera.cpp)
add_library(CameraMovement ${CMAKE_SOURCE_DIR}/src/lib/CameraMovement.cpp)
add_library(Frames ${CMAKE_SOURCE_DIR}/src/lib/Frames.cpp)
add_library(Image ${CMAKE_SOURCE_DIR}/src/lib/Image.cpp)
add_library(LodePNG ${CMAKE_SOURCE_DIR}/src/lib/LodePNG.cpp)
add_library(Material ${CMAKE_SOURCE_DIR}/src/lib/Material.cpp)
add_library(ProgressBar ${CMAKE_SOURCE_DIR}/src/lib/ProgressBar.cpp)
add_library(Random ${CMAKE_SOURCE_DIR}/src/lib/Random.cpp)
add_library(Ray ${CMAKE_SOURCE_DIR}/src/lib/Ray.cpp)
add_library(RenderAnimation ${CMAKE_SOURCE_DIR}/src/lib/RenderAnimation.cpp)
add_library(Renderer ${CMAKE_SOURCE_DIR}/src/lib/Renderer.cpp)
add_library(RenderObject ${CMAKE_SOURCE_DIR}/src/lib/RenderObject.cpp)
add_library(RenderWorld ${CMAKE_SOURCE_DIR}/src/lib/RenderWorld.cpp)
add_library(StringError ${CMAKE_SOURCE_DIR}/src/lib/StringError.cpp)
add_library(Vec3 ${CMAKE_SOURCE_DIR}/src/lib/Vec3.cpp)

# Add them to be linked to the raytracer executable
list(APPEND EXTRA_LIBS Camera)
list(APPEND EXTRA_LIBS CameraMovement)
list(APPEND EXTRA_LIBS Frames)
list(APPEND EXTRA_LIBS Image)
list(APPEND EXTRA_LIBS LodePNG)
list(APPEND EXTRA_LIBS Material)
list(APPEND EXTRA_LIBS ProgressBar)
list(APPEND EXTRA_LIBS Random)
list(APPEND EXTRA_LIBS Ray)
list(APPEND EXTRA_LIBS RenderAnimation)
list(APPEND EXTRA_LIBS Renderer)
list(APPEND EXTRA_LIBS RenderObject)
list(APPEND EXTRA_LIBS RenderWorld)
list(APPEND EXTRA_LIBS StringError)
list(APPEND EXTRA_LIBS Vec3)

# Optionally, add ThreadPool
if(THREADED)
    add_library(ThreadPool ${CMAKE_SOURCE_DIR}/src/lib/ThreadPool.cpp)
    list(APPEND EXTRA_LIBS ThreadPool)
endif()

# Finally, add the include folder for all these
list(APPEND EXTRA_INCLUDES "${CMAKE_SOURCE_DIR}/src/lib/include")