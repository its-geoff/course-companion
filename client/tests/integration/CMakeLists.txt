enable_coverage(CourseCompanion_lib)

add_executable(CourseAssignmentIntegrationTests model/CourseAssignmentIntegrationTests.cpp)
target_link_libraries(CourseAssignmentIntegrationTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME CourseAssignmentIntegrationTests COMMAND CourseAssignmentIntegrationTests)
enable_coverage(CourseAssignmentIntegrationTests)

add_executable(TermCourseIntegrationTests model/TermCourseIntegrationTests.cpp)
target_link_libraries(TermCourseIntegrationTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME TermCourseIntegrationTests COMMAND TermCourseIntegrationTests)
enable_coverage(TermCourseIntegrationTests)

add_executable(ControllerIntegrationTests controller/ControllerIntegrationTests.cpp)
target_link_libraries(ControllerIntegrationTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME ControllerIntegrationTests COMMAND ControllerIntegrationTests)
enable_coverage(ControllerIntegrationTests)

foreach(test_target
    CourseAssignmentIntegrationTests
    TermCourseIntegrationTests
    ControllerIntegrationTests
)
    set_target_properties(${test_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()