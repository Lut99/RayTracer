cmake_minimum_required(VERSION 3.10)

# Set the project name
project(RayTracer VERSION 1.0)

option(THREADED "If given, compiles with multithread options" ON)

configure_file(Config.hpp.in ${CMAKE_SOURCE_DIR}/src/lib/include/Config.hpp)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the threading library
find_package( Threads )

# Include the libraries
include(src/lib/CMakeLists.txt)

# Add the executable
add_executable(RayTracer src/RayTracer.cpp)

# Link all libraries
target_link_libraries(RayTracer PUBLIC ${EXTRA_LIBS})
target_link_libraries(RenderWorld PUBLIC RandomScene)
target_link_libraries(CameraMovement PUBLIC CameraRotation)
target_link_libraries(Renderer PUBLIC ProgressBar)
target_link_libraries(RenderObject PUBLIC RenderAnimation)
target_link_libraries(RenderObject PUBLIC RenderObjectCollection)
target_link_libraries(ThreadPool PUBLIC ${CMAKE_THREAD_LIBS_INIT})

# Add include directories
include_directories(${EXTRA_INCLUDES})