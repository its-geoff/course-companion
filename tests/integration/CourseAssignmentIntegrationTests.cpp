#include <gtest/gtest.h>
#include <sstream>      // diverts output from terminal to separate stream
#include <regex>        // regular expression matching for UUIDs
#include "model/Assignment.hpp"
#include "model/Course.hpp"
#include "utils/utils.hpp"

using namespace std::chrono_literals;

// ====================================
// SMOKE TESTS
// ====================================

TEST(CourseAssignmentIntegrationTest, AddAssignmentToCourse) {
    Course course1{"CMPE 152", "", {}, {}};
    Assignment assignment1{"Homework 1", "", "Homework", {}, false, 0.0f};

    course1.addAssignment(assignment1);

    // check that assignment was added
    ASSERT_EQ(course1.getAssignmentList().size(), 1);
}

TEST(CourseAssignmentIntegrationTest, AddMultipleAssignmentsToCourse) {
    Course course1{"CMPE 152", "", {}, {}};
    Assignment assignment1{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/12/22}, true, 100.0f};
    Assignment assignment2{"Homework 2", "", "Homework", {}, false, 0.0f};
    Assignment midterm{"Midterm", "", "Midterm", {}, false, 0.0f};
    Assignment final{"Final Exam", "", "Final Exam", {}, false, 0.0f};

    course1.addAssignment(assignment1);
    course1.addAssignment(assignment2);
    course1.addAssignment(midterm);
    course1.addAssignment(final);

    // check that all assignments were added
    ASSERT_EQ(course1.getAssignmentList().size(), 4);
}

TEST(CourseAssignmentIntegrationTest, AssignmentStoredWithCorrectFields) {
    Course course1{"CMPE 152", "", {}, {}};
    Assignment assignment1{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/12/22}, true, 100.0f};

    course1.addAssignment(assignment1);

    // check that all assignment fields are correct
    Assignment& selectedAssignment = course1.findAssignment(assignment1.getId());
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 1");
    ASSERT_EQ(selectedAssignment.getDescription(), "");
    ASSERT_EQ(selectedAssignment.getCategory(), "Homework");
    ASSERT_EQ(selectedAssignment.getDueDate(), std::chrono::year_month_day{2025y/12/22});
    ASSERT_EQ(selectedAssignment.getCompleted(), true);
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 100.0f);
}

TEST(CourseAssignmentIntegrationTest, AssignmentCategoryRecognizedByCourse) {
    Course course1{"CMPE 152", "", {}, {}};
    Assignment assignment1{"Final Exam", "", "Final Exam", std::chrono::year_month_day{2025y/12/22}, true, 94.37f};
    const std::unordered_map<std::string, float> weights = {{"Final Exam", 1.0f}};

    course1.addAssignment(assignment1);
    course1.setGradeWeights(weights);
    course1.setGradePct();

    ASSERT_FLOAT_EQ(course1.getGradePct(), 94.37f);
    ASSERT_EQ(course1.getLetterGrade(), "A");
    ASSERT_FLOAT_EQ(course1.getGpaVal(), 4.0f);
}

TEST(CourseAssignmentIntegrationTest, FinalGradeAcrossMultipleAssignments) {
    Course course1{"CMPE 152", "", {}, {}};
    Assignment assignment1{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/12/22}, true, 100.0f};
    Assignment assignment2{"Homework 2", "", "Homework", {}, true, 75.0f};
    Assignment assignment3{"Homework 3", "", "Homework", {}, true, 65.0f};
    const std::unordered_map<std::string, float> weights = {{"Homework", 1.0f}};

    course1.addAssignment(assignment1);
    course1.addAssignment(assignment2);
    course1.addAssignment(assignment3);
    course1.setGradeWeights(weights);
    course1.setGradePct();

    ASSERT_FLOAT_EQ(course1.getGradePct(), 80.0f);
}

TEST(CourseAssignmentIntegrationTest, FinalGradeAcrossMultipleCategories) {
    Course course1{"CMPE 152", "", {}, {}};
    Assignment assignment1{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/12/22}, true, 100.0f};
    Assignment assignment2{"Homework 2", "", "Homework", {}, true, 70.0f};
    Assignment midterm{"Midterm", "", "Midterm", {}, true, 78.0f};
    Assignment final{"Final Exam", "", "Final Exam", {}, true, 72.0f};

    course1.addAssignment(assignment1);
    course1.addAssignment(assignment2);
    course1.addAssignment(midterm);
    course1.addAssignment(final);
    course1.setGradePct();

    ASSERT_FLOAT_EQ(course1.getGradePct(), 77.35f);
}

TEST(CourseAssignmentIntegrationTest, AssignmentDueDatePastCourseEndDate) {
    Course course1{"CMPE 152", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}};
    Assignment assignment1{"Homework 1", "", "Homework", std::chrono::year_month_day{2026y/6/1}, false, 0.0f};

    course1.addAssignment(assignment1);

    // check assignment title and that due date is unchanged
    Assignment& selectedAssignment = course1.findAssignment(assignment1.getId());
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 1");
    ASSERT_EQ(selectedAssignment.getDueDate(), std::chrono::year_month_day{2026y/6/1});
}

TEST(CourseAssignmentIntegrationTest, AssignmentNotMutatedByCourse) {
    Course course1{"CMPE 152", "", {}, {}};
    Assignment assignment1{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/12/22}, true, 100.0f};

    course1.addAssignment(assignment1);

    // check original assignment values for mutation
    ASSERT_EQ(assignment1.getTitle(), "Homework 1");
    ASSERT_EQ(assignment1.getDescription(), "");
    ASSERT_EQ(assignment1.getCategory(), "Homework");
    ASSERT_EQ(assignment1.getDueDate(), std::chrono::year_month_day{2025y/12/22});
    ASSERT_EQ(assignment1.getCompleted(), true);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 100.0f);
}

// ====================================
// EDGE CASES
// ====================================

TEST(CourseAssignmentIntegrationTest, EmptyCategoryRedistributesWeight) {
    Course course1{"CMPE 152", "", {}, {}};
    Assignment assignment1{"Homework 1", "", "Homework", {}, true, 70.0f};
    const std::unordered_map<std::string, float> weights = {{"Homework", 0.5f}, {"Exams", 0.5f}};

    course1.addAssignment(assignment1);
    course1.setGradeWeights(weights);
    course1.setGradePct();

    // check that grade is the same as assignment grade and doesn't use empty category
    ASSERT_FLOAT_EQ(course1.getGradePct(), assignment1.getGrade());
}

TEST(CourseAssignmentIntegrationTest, CourseWithNoAssignmentsReturnsGrade) {
    Course course1{"CMPE 152", "", {}, {}};

    course1.setGradePct();

    ASSERT_FLOAT_EQ(course1.getGradePct(), 0.0f);
}