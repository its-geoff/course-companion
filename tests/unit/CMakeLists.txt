enable_coverage(CourseCompanion_lib)

# ---------------------------
# AssignmentTests executable
# ---------------------------
add_executable(AssignmentTests model/AssignmentTests.cpp)
target_link_libraries(AssignmentTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME AssignmentTests COMMAND AssignmentTests)
enable_coverage(AssignmentTests)

# ---------------------------
# CourseTests executable
# ---------------------------
add_executable(CourseTests model/CourseTests.cpp)
target_link_libraries(CourseTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME CourseTests COMMAND CourseTests)
enable_coverage(CourseTests)

# ---------------------------
# TermTests executable
# ---------------------------
add_executable(TermTests model/TermTests.cpp)
target_link_libraries(TermTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME TermTests COMMAND TermTests)
enable_coverage(TermTests)

# ---------------------------
# CliViewTests executable
# ---------------------------
add_executable(CliViewTests view/CliViewTests.cpp)
target_link_libraries(CliViewTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME CliViewTests COMMAND CliViewTests)
enable_coverage(CliViewTests)

# --------------------------------------
# AssignmentControllerTests executable
# --------------------------------------
add_executable(AssignmentControllerTests controller/AssignmentControllerTests.cpp)
target_link_libraries(AssignmentControllerTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME AssignmentControllerTests COMMAND AssignmentControllerTests)
enable_coverage(AssignmentControllerTests)

# --------------------------------
# CourseControllerTests executable
# --------------------------------
add_executable(CourseControllerTests controller/CourseControllerTests.cpp)
target_link_libraries(CourseControllerTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME CourseControllerTests COMMAND CourseControllerTests)
enable_coverage(CourseControllerTests)

# --------------------------------
# TermControllerTests executable
# --------------------------------
add_executable(TermControllerTests controller/TermControllerTests.cpp)
target_link_libraries(TermControllerTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME TermControllerTests COMMAND TermControllerTests)
enable_coverage(TermControllerTests)

# ---------------------------
# UtilsTests executable
# ---------------------------
add_executable(UtilsTests utils/UtilsTests.cpp)
target_link_libraries(UtilsTests PRIVATE CourseCompanion_lib GTest::gtest_main)
add_test(NAME UtilsTests COMMAND UtilsTests)
enable_coverage(UtilsTests)

# ---------------------------
# Set output directory
# ---------------------------
foreach(test_target 
    AssignmentTests 
    CourseTests 
    TermTests 
    CliViewTests 
    CourseControllerTests 
    TermControllerTests 
    UtilsTests
    )
    
    set_target_properties(${test_target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endforeach()