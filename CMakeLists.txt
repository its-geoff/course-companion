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
# C++ Standard
# ------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ------------------------
# Include directories
# ------------------------
include_directories(${CMAKE_SOURCE_DIR}/include)

# ------------------------
# Library containing your app logic
# ------------------------
add_library(CourseCompanion_lib
    src/Assignment.cpp
)

target_include_directories(CourseCompanion_lib PUBLIC include)

# ------------------------
# Main executable
# ------------------------
add_executable(CourseCompanion
    src/main.cpp
)

target_link_libraries(CourseCompanion PRIVATE CourseCompanion_lib)

set_target_properties(CourseCompanion PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# ------------------------
# Enable testing
# ------------------------
option(BUILD_TESTING "Build tests" ON)
if (BUILD_TESTING)
    enable_testing()

    # ---- Fetch GoogleTest ----
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )

    # Disable installing gtest into system
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)

    # ---- Add tests folder ----
    add_subdirectory(tests)
endif()