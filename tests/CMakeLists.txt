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
# ControllerTests executable
# ---------------------------
add_executable(ControllerTests integration/ControllerTests.cpp)
target_link_libraries(ControllerTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME ControllerTests COMMAND ControllerTests)

# output directory
set_target_properties(AssignmentTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)
set_target_properties(ControllerTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)