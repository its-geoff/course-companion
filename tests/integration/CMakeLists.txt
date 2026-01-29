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
set(TEST_LOG_DIR "${CMAKE_BINARY_DIR}/../../../tests/output_logs")
FetchContent_MakeAvailable(googletest)

enable_coverage(CourseCompanion_lib)

# ------------------------------------------------
# CourseAssignmentIntegrationTests executable
# ------------------------------------------------
add_executable(CourseAssignmentIntegrationTests CourseAssignmentIntegrationTests.cpp)
target_link_libraries(CourseAssignmentIntegrationTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME CourseAssignmentIntegrationTests COMMAND CourseAssignmentIntegrationTests)
enable_coverage(CourseAssignmentIntegrationTests)

# ---------------------------
# Set output directory
# ---------------------------
foreach(test_target 
    CourseAssignmentIntegrationTests 
    )
    set_target_properties(${test_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()