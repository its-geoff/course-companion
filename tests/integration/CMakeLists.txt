enable_coverage(CourseCompanion_lib)

# ------------------------------------------------
# CourseAssignmentIntegrationTests executable
# ------------------------------------------------
add_executable(CourseAssignmentIntegrationTests CourseAssignmentIntegrationTests.cpp)
target_link_libraries(CourseAssignmentIntegrationTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME CourseAssignmentIntegrationTests COMMAND CourseAssignmentIntegrationTests)
enable_coverage(CourseAssignmentIntegrationTests)

# ------------------------------------------------
# TermCourseIntegrationTests executable
# ------------------------------------------------
add_executable(TermCourseIntegrationTests TermCourseIntegrationTests.cpp)
target_link_libraries(TermCourseIntegrationTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME TermCourseIntegrationTests COMMAND TermCourseIntegrationTests)
enable_coverage(TermCourseIntegrationTests)

# ---------------------------
# Set output directory
# ---------------------------
foreach(test_target 
    CourseAssignmentIntegrationTests
    TermCourseIntegrationTests
    )
    set_target_properties(${test_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()