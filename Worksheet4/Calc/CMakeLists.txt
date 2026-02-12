cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(calc)

# Create library (default = static unless told otherwise)
add_library(maths adder.cpp)

# If static build, define macro
if (NOT BUILD_SHARED_LIBS)
    add_definitions(-Dmaths_STATIC)
endif()

# Create executable
add_executable(calc calc.cpp)

# Link executable to library
target_link_libraries(calc maths)