#include <gtest/gtest.h>
#include "controller/AssignmentController.hpp"
#include "controller/CourseController.hpp"
#include "controller/TermController.hpp"

using namespace std::chrono_literals;

// test fixture for controller integration tests
class ControllerIntegrationTests : public testing::Test {
    protected:
        TermController termController;

        void SetUp() override {
            termController.addTerm("Fall 2025", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/23}, false);
            termController.selectTerm("Fall 2025");
        }
};

// ====================================
// TERM -> COURSE CONTROLLER TESTS
// ====================================

TEST_F(ControllerIntegrationTests, CreateCourseUnderTerm) {
    CourseController& courseController = termController.getCourseController();
    courseController.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);

    auto courses = courseController.getCourseList();

    // check course list size and object data
    ASSERT_EQ(courses.size(), 1);
    Course& selectedCourse = courseController.findCourse("ENGR 195A");
    ASSERT_EQ(selectedCourse.getTitle(), "ENGR 195A");
    ASSERT_EQ(selectedCourse.getDescription(), "");
    ASSERT_EQ(selectedCourse.getStartDate(), std::chrono::year_month_day{2026y/1/2});
    ASSERT_EQ(selectedCourse.getEndDate(), std::chrono::year_month_day{2026y/5/12});
    ASSERT_EQ(selectedCourse.getNumCredits(), 3);
    ASSERT_FALSE(selectedCourse.getActive());
}

TEST_F(ControllerIntegrationTests, RemovingTermRemovesCourses) {
    CourseController& courseController = termController.getCourseController();
    courseController.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);

    // check size before deletion
    ASSERT_EQ(courseController.getCourseList().size(), 1);

    termController.removeTerm("Fall 2025");

    // verify that objects were deleted
    ASSERT_EQ(courseController.getCourseList().size(), 0);
}

// ==========================================
// COURSE -> ASSIGNMENT CONTROLLER TESTS
// ==========================================

TEST_F(ControllerIntegrationTests, CreateAssignmentUnderCourse) {
    CourseController& courseController = termController.getCourseController();
    courseController.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    courseController.selectCourse("ENGR 195A");
    
    AssignmentController& assignmentController = courseController.getAssignmentController();
    assignmentController.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    auto assignments = assignmentController.getAssignmentList();

    // check assignment list size and object data
    ASSERT_EQ(assignments.size(), 1);
    Assignment& selectedAssignment = assignmentController.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 1");
    ASSERT_EQ(selectedAssignment.getDescription(), "");
    ASSERT_EQ(selectedAssignment.getCategory(), "Homework");
    ASSERT_EQ(selectedAssignment.getDueDate(), std::chrono::year_month_day{2026y/1/12});
    ASSERT_TRUE(selectedAssignment.getCompleted());
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 90.0f);
}

TEST_F(ControllerIntegrationTests, RemovingCourseRemovesAssignments) {
    CourseController& courseController = termController.getCourseController();
    courseController.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    courseController.selectCourse("ENGR 195A");
    
    AssignmentController& assignmentController = courseController.getAssignmentController();
    assignmentController.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    // check size before deletion
    ASSERT_EQ(assignmentController.getAssignmentList().size(), 1);

    courseController.removeCourse("ENGR 195A");

    // verify that objects were deleted
    ASSERT_EQ(assignmentController.getAssignmentList().size(), 0);
}

// ==================================================
// TERM -> COURSE -> ASSIGNMENT CONTROLLER TESTS
// ==================================================

TEST_F(ControllerIntegrationTests, FullHierarchyCreation) {
    CourseController& courseController = termController.getCourseController();
    courseController.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    courseController.selectCourse("ENGR 195A");
    
    AssignmentController& assignmentController = courseController.getAssignmentController();
    assignmentController.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    assignmentController.addAssignment("Midterm 1", "", "Midterm", std::chrono::year_month_day{2026y/1/16}, true, 70.0f);

    ASSERT_EQ(courseController.getCourseList().size(), 1);
    ASSERT_EQ(assignmentController.getAssignmentList().size(), 2);
}

TEST_F(ControllerIntegrationTests, DeleteTermRemovesAllChildren) {
    CourseController& courseController = termController.getCourseController();
    courseController.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    courseController.selectCourse("ENGR 195A");
    
    AssignmentController& assignmentController = courseController.getAssignmentController();
    assignmentController.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    // check size before deletion
    ASSERT_EQ(courseController.getCourseList().size(), 1);
    ASSERT_EQ(assignmentController.getAssignmentList().size(), 1);

    termController.removeTerm("Fall 2025");

    // verify that objects were deleted
    ASSERT_EQ(courseController.getCourseList().size(), 0);
    ASSERT_EQ(assignmentController.getAssignmentList().size(), 0);
}

// ====================================
// MUTATION AND SYNC TESTS
// ====================================

// TO-DO: add after settings with grade weight changes
// TEST_F(ControllerIntegrationTests, AssignmentWeightChangesGrade) {
//     CourseController& courseController = termController.getCourseController();
//     courseController.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
//     courseController.selectCourse("ENGR 195A");

//     AssignmentController& assignmentController = courseController.getAssignmentController();
//     assignmentController.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
//     assignmentController.addAssignment("Midterm 1", "", "Midterm", std::chrono::year_month_day{2026y/1/16}, true, 70.0f);

    
// }