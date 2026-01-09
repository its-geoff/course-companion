cmake_minimum_required(VERSION 3.20)

# ------------------------
# Project metadata
# ------------------------
project(CourseCompanion
    VERSION 0.2.0
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
    src/controller/CourseController.cpp
    src/controller/TermController.cpp
    src/utils/utils.cpp
)

target_include_directories(CourseCompanion_lib PUBLIC include)
target_link_libraries(CourseCompanion_lib PRIVATE $<$<PLATFORM_ID:Linux>:uuid>)     # only link on Linux

# -------------------------------
# Helper function for coverage
# -------------------------------
function(enable_coverage target)
    if(ENABLE_COVERAGE AND CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang")
        target_compile_options(${target} PRIVATE
            -fprofile-instr-generate -fcoverage-mapping
        )
        target_link_options(${target} PRIVATE
            -fprofile-instr-generate
        )
    endif()
endfunction()

# ------------------------
# Main executable
# ------------------------
if(NOT BUILD_TESTING)
    add_executable(CourseCompanion src/main.cpp)
    target_link_libraries(CourseCompanion PRIVATE CourseCompanion_lib)
    
    set_target_properties(CourseCompanion PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endif()

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()