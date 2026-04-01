cmake_minimum_required(VERSION 3.20)

# ------------------------
# Project metadata
# ------------------------
project(CourseCompanion
    VERSION 0.4.0
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
# Core library (no Qt, no CLI view)
# Used by both GUI, CLI, and tests
# ------------------------------------------
add_library(CourseCompanion_lib
    src/model/Assignment.cpp
    src/model/Course.cpp
    src/model/Term.cpp
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
list(APPEND CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(MYSQL_CONCPP_STATIC ON)
find_package(mysql-concpp REQUIRED)

get_target_property(_mysql_inc mysql::concpp INTERFACE_INCLUDE_DIRECTORIES)
message(STATUS "mysql include dirs: ${_mysql_inc}")

target_include_directories(CourseCompanion_lib PUBLIC
    ${_mysql_inc}
)

target_link_libraries(CourseCompanion_lib
    PUBLIC
        mysql::concpp-xdevapi-static
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
# Main executable (GUI)
# ------------------------
if(NOT BUILD_TESTING)
    find_package(Qt6 REQUIRED COMPONENTS Widgets)

    add_library(CourseCompanion_gui
        include/view/qt/MainWindow.hpp
        src/view/qt/MainWindow.cpp
    )

    set_target_properties(CourseCompanion_gui PROPERTIES
        AUTOMOC ON
        AUTOUIC ON
        AUTORCC ON
    )

    target_link_libraries(CourseCompanion_gui
        PUBLIC
            CourseCompanion_lib
            Qt6::Widgets
    )

    add_executable(CourseCompanion src/main.cpp)
    target_compile_definitions(CourseCompanion PRIVATE USE_GUI)
    target_link_libraries(CourseCompanion PRIVATE CourseCompanion_gui)

    set_target_properties(CourseCompanion PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        AUTOMOC ON
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
    )
endif()

# ------------------------
# Test build (CLI)
# ------------------------
if(BUILD_TESTING)
    enable_testing()

    add_library(CourseCompanion_cli
        src/view/cli/CliView.cpp
    )

    target_link_libraries(CourseCompanion_cli
        PUBLIC CourseCompanion_lib
    )

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

    add_subdirectory(tests/unit)
    add_subdirectory(tests/integration)
endif()