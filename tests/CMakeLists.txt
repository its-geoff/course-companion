# enable testing
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
FetchContent_MakeAvailable(googletest)

# ---------------------------
# AssignmentTests executable
# ---------------------------
add_executable(AssignmentTests unit/AssignmentTests.cpp)
target_link_libraries(AssignmentTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME AssignmentTests COMMAND AssignmentTests)

# ---------------------------
# CourseTests executable
# ---------------------------
add_executable(CourseTests unit/CourseTests.cpp)
target_link_libraries(CourseTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME CourseTests COMMAND CourseTests)

# ---------------------------
# UtilsTests executable
# ---------------------------
add_executable(UtilsTests unit/UtilsTests.cpp)
target_link_libraries(UtilsTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME UtilsTests COMMAND UtilsTests)

# ---------------------------
# Code coverage
# ---------------------------
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    foreach(test_target AssignmentTests CourseTests UtilsTests)
        target_compile_options(${test_target} PRIVATE --coverage)
        target_link_options(${test_target} PRIVATE --coverage)
    endforeach()
endif()

# output directory
set_target_properties(AssignmentTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(CourseTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(UtilsTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)