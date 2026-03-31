cmake_minimum_required(VERSION 3.20)

# ------------------------
# Project metadata
# ------------------------
project(CourseCompanion
    VERSION 0.3.0
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

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# ------------------------
# Include directories
# ------------------------
include_directories(${CMAKE_SOURCE_DIR}/include)

# ------------------------------------------
# Library containing app logic for testing
# ------------------------------------------
add_library(CourseCompanion_lib
    src/model/Assignment.cpp
    src/model/Course.cpp
    src/model/Term.cpp
    src/view/CliView.cpp
    src/controller/AssignmentController.cpp
    src/controller/CourseController.cpp
    src/controller/TermController.cpp
    src/db/DatabaseConnection.cpp
    src/db/AssignmentRepository.cpp
    src/db/TermRepository.cpp
    src/db/CourseRepository.cpp
    src/utils/utils.cpp
)

target_include_directories(CourseCompanion_lib PUBLIC include)
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})  # allows find_package to locate Conan-generated configs
set(MYSQL_CONCPP_STATIC ON)
find_package(mysql-concpp REQUIRED)

# mysql::concpp-xdevapi-static is a thin INTERFACE target that delegates to mysql::concpp;
# INTERFACE_INCLUDE_DIRECTORIES lives on mysql::concpp, so we read from there directly
get_target_property(_mysql_inc mysql::concpp INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "mysql include dirs: ${_mysql_inc}")

target_include_directories(CourseCompanion_lib PUBLIC
    ${_mysql_inc}
)

target_link_libraries(CourseCompanion_lib
    PUBLIC
        mysql::concpp-xdevapi-static    # PUBLIC so test targets inherit includes transitively
    PRIVATE
        $<$<PLATFORM_ID:Linux>:uuid>
)

# -------------------------------
# Helper function for coverage
# -------------------------------
function(enable_coverage target)
    if(NOT ENABLE_COVERAGE)
        return()
    endif()

    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
        target_compile_options(${target} PRIVATE
            -fprofile-instr-generate -fcoverage-mapping
        )
        target_link_options(${target} PRIVATE
            -fprofile-instr-generate
        )
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
        target_compile_options(${target} PRIVATE
            -g -O0 --coverage
        )
        target_link_options(${target} PRIVATE
            --coverage
        )
    else()
        message(WARNING "Coverage not supported for compiler ${CMAKE_CXX_COMPILER_ID}")
    endif()
endfunction()

# ------------------------
# Main executable
# ------------------------
if(NOT BUILD_TESTING)
    add_executable(CourseCompanion src/main.cpp)
    target_link_libraries(CourseCompanion PRIVATE
        CourseCompanion_lib     # mysql includes and uuid are transitive via CourseCompanion_lib
    )

    set_target_properties(CourseCompanion PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endif()

if(BUILD_TESTING)
    enable_testing()

    # fetch GoogleTest
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
        DOWNLOAD_EXTRACT_TIMESTAMP TRUE
    )
    set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
    set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
    set(TEST_LOG_DIR "${CMAKE_BINARY_DIR}/../../../tests/output_logs")
    FetchContent_MakeAvailable(googletest)

    # add each test type
    add_subdirectory(tests/unit)
    add_subdirectory(tests/integration)
endif()