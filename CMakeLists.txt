cmake_minimum_required(VERSION 3.20)

# ------------------------
# Project metadata
# ------------------------
project(CourseCompanion
    VERSION 0.1.0
    DESCRIPTION "Homework tracker that allows students to track homework assignments, estimate grades, and organize their class schedules."
    LANGUAGES CXX
)

# ------------------------
# Specify C++ standard
# ------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------
# Include Conan
# ------------------------
# Assumes conan install was run in the build folder
if(EXISTS "${CMAKE_BINARY_DIR}/conanbuildinfo.cmake")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif()

# ------------------------
# Include directories
# ------------------------
include_directories(${CMAKE_SOURCE_DIR}/include)

# ------------------------
# Source files
# ------------------------
set(SOURCES
    src/main.cpp
    src/Assignment.cpp
)

# ------------------------
# Add executable
# ------------------------
add_executable(CourseCompanion ${SOURCES})

# ------------------------
# Link dependencies from Conan
# ------------------------


# ------------------------
# Optional: output directories
# ------------------------
set_target_properties(CourseCompanion PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
