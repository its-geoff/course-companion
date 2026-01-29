// #include <gtest/gtest.h>
// #include <sstream>      // diverts output from terminal to separate stream
// #include <regex>        // regular expression matching for UUIDs
// #include "model/Assignment.hpp"
// #include "model/Course.hpp"
// #include "utils/utils.hpp"

// using namespace std::chrono_literals;

// // ====================================
// // SMOKE TESTS
// // ====================================

// TEST(CourseAssignmentIntegrationTest, AddAssignmentToCourse) {
//     Course course1{"CMPE 152", "", {}, {}};
//     Assignment assignment1{"Homework 1", "", "Homework", {}, false, 0.0f};

//     course1.addAssignment(assignment1);

//     // check that assignment was added
//     ASSERT_EQ(course1.getAssignmentList().size(), 1);
// }

// TEST(CourseAssignmentIntegrationTest, AddMultipleAssignmentsToCourse) {
//     Course course1{"CMPE 152", "", {}, {}};
//     Assignment assignment1{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/12/22}, true, 100.0f};
//     Assignment assignment2{"Homework 2", "", "Homework", {}, false, 0.0f};
//     Assignment midterm{"Midterm", "", "Midterm", {}, false, 0.0f};
//     Assignment final{"Final Exam", "", "Final Exam", {}, false, 0.0f};

//     course1.addAssignment(assignment1);
//     course1.addAssignment(assignment2);
//     course1.addAssignment(midterm);
//     course1.addAssignment(final);

//     // check that all assignments were added
//     ASSERT_EQ(course1.getAssignmentList().size(), 4);
// }

// TEST(CourseAssignmentIntegrationTest, AssignmentAppearsInCourseOutput) {
//     Course course1{"CMPE 152", "", {}, {}};
//     Assignment assignment1{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/12/22}, true, 100.0f};

//     course1.addAssignment(assignment1);

//     // check that all assignment fields are correct
//     Assignment& selectedAssignment = course1.findAssignment(assignment1.getId());
//     ASSERT_EQ(selectedAssignment.getTitle(), "Homework 1");
//     ASSERT_EQ(selectedAssignment.getDescription(), "");
//     ASSERT_EQ(selectedAssignment.getDueDate(), std::chrono::year_month_day{2025y/12/22});
//     ASSERT_EQ(selectedAssignment.getCompleted(), true);
//     ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 100.0f);
// }

// TEST(CourseAssignmentIntegrationTest, AssignmentCategoryRecognizedByCourse) {
//     Course course1{"CMPE 152", "", {}, {}};
//     Assignment assignment1{"Final Exam", "", "Final Exam", std::chrono::year_month_day{2025y/12/22}, true, 94.37f};

//     course1.addAssignment(assignment1);
//     course1.setGradePct();

//     // check that all values align
//     ASSERT_FLOAT_EQ(course1.getGradePct(), 94.37f);
//     ASSERT_EQ(course1.getLetterGrade(), "A");
//     ASSERT_FLOAT_EQ(course1.getGpaVal(), 4.0f);
// }

// ====================================
// EDGE CASES
// ====================================