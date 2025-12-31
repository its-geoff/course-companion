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
set(TEST_LOG_DIR "${CMAKE_BINARY_DIR}/../../tests/output_logs")
FetchContent_MakeAvailable(googletest)

enable_coverage(CourseCompanion_lib)

# ---------------------------
# AssignmentTests executable
# ---------------------------
add_executable(AssignmentTests unit/AssignmentTests.cpp)
target_link_libraries(AssignmentTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME AssignmentTests COMMAND AssignmentTests)
enable_coverage(AssignmentTests)

# ---------------------------
# CourseTests executable
# ---------------------------
add_executable(CourseTests unit/CourseTests.cpp)
target_link_libraries(CourseTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME CourseTests COMMAND CourseTests)
enable_coverage(CourseTests)

# ---------------------------
# TermTests executable
# ---------------------------
add_executable(TermTests unit/TermTests.cpp)
target_link_libraries(TermTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME TermTests COMMAND TermTests)
enable_coverage(TermTests)

# ---------------------------
# CliViewTests executable
# ---------------------------
add_executable(CliViewTests unit/CliViewTests.cpp)
target_link_libraries(CliViewTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME CliViewTests COMMAND CliViewTests)
enable_coverage(CliViewTests)

# ---------------------------
# UtilsTests executable
# ---------------------------
add_executable(UtilsTests unit/UtilsTests.cpp)
target_link_libraries(UtilsTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME UtilsTests COMMAND UtilsTests)
enable_coverage(UtilsTests)

# ---------------------------
# Set output directory
# ---------------------------
foreach(test_target AssignmentTests CourseTests TermTests CliViewTests UtilsTests)
    set_target_properties(${test_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()