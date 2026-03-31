enable_coverage(CourseCompanion_lib)

# ------------------------------------------------
# CourseAssignmentIntegrationTests executable
# ------------------------------------------------
add_executable(CourseAssignmentIntegrationTests model/CourseAssignmentIntegrationTests.cpp)
target_link_libraries(CourseAssignmentIntegrationTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME CourseAssignmentIntegrationTests COMMAND CourseAssignmentIntegrationTests)
enable_coverage(CourseAssignmentIntegrationTests)

# ------------------------------------------------
# TermCourseIntegrationTests executable
# ------------------------------------------------
add_executable(TermCourseIntegrationTests model/TermCourseIntegrationTests.cpp)
target_link_libraries(TermCourseIntegrationTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME TermCourseIntegrationTests COMMAND TermCourseIntegrationTests)
enable_coverage(TermCourseIntegrationTests)

# ------------------------------------------------
# ControllerIntegrationTests executable
# ------------------------------------------------
add_executable(ControllerIntegrationTests controller/ControllerIntegrationTests.cpp)
target_link_libraries(ControllerIntegrationTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME ControllerIntegrationTests COMMAND ControllerIntegrationTests)
enable_coverage(ControllerIntegrationTests)

# --------------------------------
# AssignmentRepositoryTests executable
# --------------------------------
add_executable(AssignmentRepositoryTests db/AssignmentRepositoryTests.cpp)
target_link_libraries(AssignmentRepositoryTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME AssignmentRepositoryTests COMMAND AssignmentRepositoryTests)
enable_coverage(AssignmentRepositoryTests)

# --------------------------------
# CourseRepositoryTests executable
# --------------------------------
add_executable(CourseRepositoryTests db/CourseRepositoryTests.cpp)
target_link_libraries(CourseRepositoryTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME CourseRepositoryTests COMMAND CourseRepositoryTests)
enable_coverage(CourseRepositoryTests)

# --------------------------------
# TermRepositoryTests executable
# --------------------------------
add_executable(TermRepositoryTests db/TermRepositoryTests.cpp)
target_link_libraries(TermRepositoryTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME TermRepositoryTests COMMAND TermRepositoryTests)
enable_coverage(TermRepositoryTests)

# ---------------------------
# Set output directory
# ---------------------------
foreach(test_target 
    CourseAssignmentIntegrationTests
    TermCourseIntegrationTests
    ControllerIntegrationTests
    AssignmentRepositoryTests
    CourseRepositoryTests
    TermRepositoryTests
    )

    set_target_properties(${test_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()