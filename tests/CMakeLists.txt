# Create test executable
add_executable(AssignmentTests
    AssignmentTests.cpp
)

# Link test executable with project code
target_link_libraries(AssignmentTests
    PRIVATE
        CourseCompanion_lib
        GTest::gtest_main
)

add_test(NAME AssignmentTests COMMAND AssignmentTests)

set_target_properties(AssignmentTests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)