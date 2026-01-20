#include <gtest/gtest.h>
#include <sstream>      // diverts output from terminal to separate stream
#include <regex>        // regular expression matching for UUIDs
#include "controller/AssignmentController.hpp"
#include "utils/utils.hpp"

using namespace std::chrono_literals;

// test fixture for class AssignmentController
class AssignmentControllerTest : public testing::Test {
    protected:
        Course course {"ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false};
        AssignmentController controller{course};
};

// ====================================
// GETTER SMOKE TESTS
// ====================================

TEST_F(AssignmentControllerTest, AssignmentListGetter) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", "Homework", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

    const std::unordered_map<std::string, Assignment> &listOfAssignments = controller.getAssignmentList();
    ASSERT_EQ(listOfAssignments.size(), 2);

    // check if both added assignments are in the list
    std::string id1 = controller.getAssignmentId("Homework 1");
    auto it1 = listOfAssignments.find(id1);
    ASSERT_TRUE(it1 != listOfAssignments.end());

    std::string id2 = controller.getAssignmentId("Homework 2");
    auto it2 = listOfAssignments.find(id2);
    ASSERT_TRUE(it2 != listOfAssignments.end());
}

TEST_F(AssignmentControllerTest, AssignmentIdGetter) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", "Homework", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

    std::string id = controller.getAssignmentId("Homework 1");

    // find and replace UUID
    std::regex uuidRegex("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    id = std::regex_replace(id, uuidRegex, "<UUID>");

    // if the UUID is in the correct format, it should match this
    ASSERT_EQ(id, "<UUID>");
}

// ====================================
// FUNCTION SMOKE TESTS
// ====================================

TEST_F(AssignmentControllerTest, AddAssignment) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 1");
    ASSERT_EQ(selectedAssignment.getDescription(), "");
    ASSERT_EQ(selectedAssignment.getCategory(), "Homework");
    ASSERT_EQ(selectedAssignment.getDueDate(), std::chrono::year_month_day{2026y/1/12});
    ASSERT_TRUE(selectedAssignment.getCompleted());
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 90.0f);
}

TEST_F(AssignmentControllerTest, EditTitle) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    std::string id = controller.getAssignmentId("Homework 1");
    controller.editTitle(id, "Homework 3");

    // check that title has been edited and title -> id mapping is correct
    const Assignment& selectedAssignment = controller.findAssignment("Homework 3");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 3");
    ASSERT_EQ(controller.getAssignmentId("Homework 3"), id);
}

TEST_F(AssignmentControllerTest, EditDescription) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    std::string id = controller.getAssignmentId("Homework 1");
    controller.editDescription(id, "Linked lists and hash maps");

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getDescription(), "Linked lists and hash maps");
}

TEST_F(AssignmentControllerTest, EditCategory) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    std::string id = controller.getAssignmentId("Homework 1");
    controller.editCategory(id, "Midterm");

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getCategory(), "Midterm");
}

TEST_F(AssignmentControllerTest, EditDueDate) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    std::string id = controller.getAssignmentId("Homework 1");
    controller.editDueDate(id, std::chrono::year_month_day{2026y/1/15});

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getDueDate(), std::chrono::year_month_day{2026y/1/15});
}

TEST_F(AssignmentControllerTest, AddGradePercentage) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, false, 0.0f);

    controller.addGrade("Homework 1", 89.92f);

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 1");
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 89.92f);
    ASSERT_TRUE(selectedAssignment.getCompleted());
}

TEST_F(AssignmentControllerTest, AddGradePoints) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, false, 0.0f);

    controller.addGrade("Homework 1", 18, 20);

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 1");
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 90.0f);
    ASSERT_TRUE(selectedAssignment.getCompleted());
}

TEST_F(AssignmentControllerTest, RemoveGrade) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    controller.removeGrade("Homework 1");

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 1");
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 0.0f);
    ASSERT_FALSE(selectedAssignment.getCompleted());
}

TEST_F(AssignmentControllerTest, RemoveAssignment) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", "Homework", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

    controller.removeAssignment("Homework 1");

    const std::unordered_map<std::string, Assignment>& listOfAssignments = controller.getAssignmentList();
    ASSERT_EQ(listOfAssignments.size(), 1);

    // throw out of range since the assignment is not in the list
    ASSERT_THROW(controller.getAssignmentId("Homework 1"), std::out_of_range);

    std::string id2 = controller.getAssignmentId("Homework 2");
    auto it2 = listOfAssignments.find(id2);
    ASSERT_TRUE(it2 != listOfAssignments.end());
}

TEST_F(AssignmentControllerTest, FindAssignmentConst) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", "Homework", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

    const AssignmentController& constController = controller;
    const Assignment& selectedAssignment = constController.findAssignment("Homework 2");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 2");
    ASSERT_EQ(selectedAssignment.getDescription(), "Functions and variables");
    ASSERT_EQ(selectedAssignment.getDueDate(), std::chrono::year_month_day{2026y/1/22});
    ASSERT_FALSE(selectedAssignment.getCompleted());
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 0.0);
}

TEST_F(AssignmentControllerTest, FindAssignmentNonConst) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", "Homework", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

    const Assignment& selectedAssignment = controller.findAssignment("Homework 2");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 2");
    ASSERT_EQ(selectedAssignment.getDescription(), "Functions and variables");
    ASSERT_EQ(selectedAssignment.getDueDate(), std::chrono::year_month_day{2026y/1/22});
    ASSERT_FALSE(selectedAssignment.getCompleted());
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 0.0);
}

// ====================================
// GETTER EDGE CASES
// ====================================

TEST_F(AssignmentControllerTest, AssignmentListGetterEmpty) {
    const std::unordered_map<std::string, Assignment>& listOfAssignments = controller.getAssignmentList();
    ASSERT_EQ(listOfAssignments.size(), 0);
}

TEST_F(AssignmentControllerTest, AssignmentIdGetterNotFound) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", "Homework", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

    // out of range error since assignment cannot be found
    ASSERT_THROW(controller.getAssignmentId("Homework 4"), std::out_of_range);
}

// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST_F(AssignmentControllerTest, AddAssignmentAlreadyExists) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    // logic error since assignment already exists
    ASSERT_THROW(controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f), std::logic_error);
}

TEST_F(AssignmentControllerTest, EditTitleAlreadyExists) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    std::string id = controller.getAssignmentId("Homework 1");

    // logic error since assignment already exists (title not changed)
    ASSERT_THROW(controller.editTitle(id, "Homework 1"), std::logic_error);
}

TEST_F(AssignmentControllerTest, EditTitleAlreadyExistsDifferentCase) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    std::string id = controller.getAssignmentId("Homework 1");

    // logic error since assignment already exists (title not changed)
    ASSERT_THROW(controller.editTitle(id, "HOMEWORK 1"), std::logic_error);
}

TEST_F(AssignmentControllerTest, EditTitleEmpty) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    std::string id = controller.getAssignmentId("Homework 1");

    // invalid argument since title is empty
    ASSERT_THROW(controller.editTitle(id, ""), std::invalid_argument);
}

TEST_F(AssignmentControllerTest, EditCategoryEmpty) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    std::string id = controller.getAssignmentId("Homework 1");

    // invalid argument since category is empty
    ASSERT_THROW(controller.editCategory(id, ""), std::invalid_argument);
}

TEST_F(AssignmentControllerTest, EditDueDateInvalid) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    std::string id = controller.getAssignmentId("Homework 1");

    // invalid argument since due date doesn't exist
    ASSERT_THROW(controller.editDueDate(id, std::chrono::year_month_day{2026y/3/33}), std::invalid_argument);
}

TEST_F(AssignmentControllerTest, AddGradeAssignmentNotFound) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, false, 0.0f);
    // out of range since assignment cannot be found
    ASSERT_THROW(controller.addGrade("Homework 3", 89.92f), std::out_of_range);
}

TEST_F(AssignmentControllerTest, AddGradePercentageOutOfRangeLow) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, false, 0.0f);
    // out of range since grade is below 0
    ASSERT_THROW(controller.addGrade("Homework 1", -2.63f), std::out_of_range);
}

TEST_F(AssignmentControllerTest, AddGradePercentageOutOfRangeHigh) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, false, 0.0f);
    // out of range since grade is above 150
    ASSERT_THROW(controller.addGrade("Homework 1", 162.11f), std::out_of_range);
}

TEST_F(AssignmentControllerTest, AddGradePercentageBoundaryLow) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, false, 0.0f);

    controller.addGrade("Homework 1", 0.0f);

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 1");
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 0.0f);
    ASSERT_TRUE(selectedAssignment.getCompleted());
}

TEST_F(AssignmentControllerTest, AddGradePercentageBoundaryHigh) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, false, 0.0f);

    controller.addGrade("Homework 1", 150.0f);

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 1");
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 150.0f);
    ASSERT_TRUE(selectedAssignment.getCompleted());
}

TEST_F(AssignmentControllerTest, AddGradePointsOutOfRangeLow) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, false, 0.0f);
    // out of range since grade is below 0
    ASSERT_THROW(controller.addGrade("Homework 1", -12, 20), std::out_of_range);
}

TEST_F(AssignmentControllerTest, AddGradePointsOutOfRangeHigh) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, false, 0.0f);
    // out of range since grade is above 150
    ASSERT_THROW(controller.addGrade("Homework 1", 40, 20), std::out_of_range);
}

TEST_F(AssignmentControllerTest, AddGradePointsBoundaryLow) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, false, 0.0f);

    controller.addGrade("Homework 1", 0, 20);

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 1");
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 0.0f);
    ASSERT_TRUE(selectedAssignment.getCompleted());
}

TEST_F(AssignmentControllerTest, AddGradePointsBoundaryHigh) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, false, 0.0f);

    controller.addGrade("Homework 1", 30, 20);

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 1");
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 150.0f);
    ASSERT_TRUE(selectedAssignment.getCompleted());
}

TEST_F(AssignmentControllerTest, RemoveGradeAssignmentNotFound) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, false, 0.0f);
    // out of range since assignment cannot be found
    ASSERT_THROW(controller.removeGrade("Homework 3"), std::out_of_range);
}

TEST_F(AssignmentControllerTest, RemoveAssignmentNotFound) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", "Homework", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

    // out of range since assignment cannot be found
    ASSERT_THROW(controller.removeAssignment("Homework 4"), std::out_of_range);
}

TEST_F(AssignmentControllerTest, FindAssignmentConstNotFound) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", "Homework", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

    // out of range error since assignment cannot be found
    const AssignmentController& constController = controller;
    ASSERT_THROW(constController.findAssignment("Homework 4"), std::out_of_range);
}

TEST_F(AssignmentControllerTest, FindAssignmentNonConstNotFound) {
    controller.addAssignment("Homework 1", "", "Homework", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", "Homework", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

    // out of range error since assignment cannot be found
    ASSERT_THROW(controller.findAssignment("Homework 4"), std::out_of_range);
}