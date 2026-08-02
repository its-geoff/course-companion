cmake_minimum_required(VERSION 3.20)

project(CourseCompanion
    VERSION 0.4.1
    DESCRIPTION "Homework tracker that allows students to track homework assignments, estimate grades, and organize class schedules."
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ENABLE_COVERAGE "Enable code test coverage" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

find_package(Qt6 REQUIRED COMPONENTS Core)
set(CMAKE_AUTOMOC ON)

add_library(CourseCompanion_lib
    src/controller/AssignmentController.cpp
    src/controller/CourseController.cpp
    src/controller/TermController.cpp
    src/model/Assignment.cpp
    src/model/Course.cpp
    src/model/Term.cpp
    src/utils/utils.cpp
)

target_sources(CourseCompanion_lib PRIVATE
    include/controller/AssignmentController.hpp
    include/controller/CourseController.hpp
    include/controller/TermController.hpp
)

target_include_directories(CourseCompanion_lib PUBLIC include)
target_link_libraries(CourseCompanion_lib
    PUBLIC
        Qt6::Core
    PRIVATE
        $<$<PLATFORM_ID:Linux>:uuid>
)

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

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_library(CourseCompanion_gui OBJECT
    src/view/cli/CliView.cpp
    src/view/qt/AssignmentView.cpp
    src/view/qt/CourseView.cpp
    src/view/qt/FormDialog.cpp
    src/view/qt/MainWindow.cpp
    src/view/qt/TermView.cpp
)

target_include_directories(CourseCompanion_gui PUBLIC include)

set_target_properties(CourseCompanion_gui PROPERTIES
    AUTOMOC_MOC_OPTIONS ""
)
target_sources(CourseCompanion_gui PRIVATE
    include/view/qt/AssignmentView.hpp
    include/view/qt/CourseView.hpp
    include/view/qt/FormDialog.hpp
    include/view/qt/MainWindow.hpp
    include/view/qt/PaletteWatcher.hpp
    include/view/qt/StyleManager.hpp
    include/view/qt/TermView.hpp
)

target_link_libraries(CourseCompanion_gui
    PUBLIC
        CourseCompanion_lib
        Qt6::Widgets
)

if(NOT BUILD_TESTING)
    add_executable(CourseCompanion src/main.cpp)
    target_include_directories(CourseCompanion PRIVATE include)
    target_link_libraries(CourseCompanion PRIVATE CourseCompanion_gui CourseCompanion_lib)

    set_target_properties(CourseCompanion PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
        WIN32_EXECUTABLE ON
        MACOSX_BUNDLE ON
    )
endif()

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