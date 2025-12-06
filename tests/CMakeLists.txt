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

# add test executables
add_executable(tests 
    integration/ControllerTests.cpp
    unit/AssignmentTests.cpp
)
target_link_libraries(tests PRIVATE CourseCompanion_lib GTest::gtest_main)

# auto-discover tests
include(GoogleTest)
gtest_discover_tests(tests)

# set output directory
set_target_properties(tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)