cmake_minimum_required(VERSION 3.20)

# ------------------------
# Project metadata
# ------------------------
project(CourseCompanion
    VERSION 0.1.0
    DESCRIPTION "Homework tracker that allows students to track homework assignments, estimate grades, and organize class schedules."
    LANGUAGES CXX
)

# ------------------------
# C++ standard
# ------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable code test coverage" OFF)

if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
    message(STATUS "Enabling coverage flags for Clang/AppleClang")
    add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
    add_link_options(-fprofile-instr-generate)
endif()

# ------------------------
# Include directories
# ------------------------
include_directories(${CMAKE_SOURCE_DIR}/include)

# ------------------------
# Library containing app logic
# ------------------------
add_library(CourseCompanion_lib
    src/models/Assignment.cpp
    src/models/Course.cpp
    src/app/Controller.cpp
    src/app/utils.cpp
)

target_include_directories(CourseCompanion_lib PUBLIC include)

# ------------------------
# Main executable
# ------------------------
add_executable(CourseCompanion
    src/app/main.cpp
    src/app/utils.cpp
)

target_link_libraries(CourseCompanion PRIVATE CourseCompanion_lib)

set_target_properties(CourseCompanion PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

option(BUILD_TESTING "Build tests" OFF)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()