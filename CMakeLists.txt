cmake_minimum_required(VERSION 3.10)

# Set the project name
project(RayTracer VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(RayTracer tutorial.cxx)

target_include_directories(RayTracer PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           )