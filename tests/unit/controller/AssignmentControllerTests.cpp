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
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

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
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

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
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 1");
    ASSERT_EQ(selectedAssignment.getDescription(), "");
    ASSERT_EQ(selectedAssignment.getDueDate(), std::chrono::year_month_day{2026y/1/12});
    ASSERT_TRUE(selectedAssignment.getCompleted());
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 90.0f);
}

TEST_F(AssignmentControllerTest, EditTitle) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    std::string id = controller.getAssignmentId("Homework 1");
    controller.editTitle(id, "Homework 3");

    // check that title has been edited and title -> id mapping is correct
    const Assignment& selectedAssignment = controller.findAssignment("Homework 3");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 3");
    ASSERT_EQ(controller.getAssignmentId("Homework 3"), id);
}

TEST_F(AssignmentControllerTest, EditDescription) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    std::string id = controller.getAssignmentId("Homework 1");
    controller.editDescription(id, "Linked lists and hash maps");

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getDescription(), "Linked lists and hash maps");
}

TEST_F(AssignmentControllerTest, EditDueDate) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    std::string id = controller.getAssignmentId("Homework 1");
    controller.editDueDate(id, std::chrono::year_month_day{2026y/1/15});

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_EQ(selectedAssignment.getDueDate(), std::chrono::year_month_day{2026y/1/15});
}

TEST_F(AssignmentControllerTest, EditCompleted) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    std::string id = controller.getAssignmentId("Homework 1");
    controller.editCompleted(id, false);

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_FALSE(selectedAssignment.getCompleted());
}

TEST_F(AssignmentControllerTest, EditGrade) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);

    std::string id = controller.getAssignmentId("Homework 1");
    controller.editGrade(id, 95.0f);

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 95.0f);
}

TEST_F(AssignmentControllerTest, RemoveAssignment) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

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
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

    const AssignmentController& constController = controller;
    const Assignment& selectedAssignment = constController.findAssignment("Homework 2");
    ASSERT_EQ(selectedAssignment.getTitle(), "Homework 2");
    ASSERT_EQ(selectedAssignment.getDescription(), "Functions and variables");
    ASSERT_EQ(selectedAssignment.getDueDate(), std::chrono::year_month_day{2026y/1/22});
    ASSERT_FALSE(selectedAssignment.getCompleted());
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 0.0);
}

TEST_F(AssignmentControllerTest, FindAssignmentNonConst) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

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
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

    // out of range error since assignment cannot be found
    ASSERT_THROW(controller.getAssignmentId("Homework 4"), std::out_of_range);
}

// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST_F(AssignmentControllerTest, AddAssignmentAlreadyExists) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    // logic error since assignment already exists
    ASSERT_THROW(controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f), std::logic_error);
}

TEST_F(AssignmentControllerTest, EditTitleAlreadyExists) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    std::string id = controller.getAssignmentId("Homework 1");

    // logic error since assignment already exists (title not changed)
    ASSERT_THROW(controller.editTitle(id, "Homework 1"), std::logic_error);
}

TEST_F(AssignmentControllerTest, EditTitleAlreadyExistsDifferentCase) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    std::string id = controller.getAssignmentId("Homework 1");

    // logic error since assignment already exists (title not changed)
    ASSERT_THROW(controller.editTitle(id, "HOMEWORK 1"), std::logic_error);
}

TEST_F(AssignmentControllerTest, EditTitleEmpty) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    std::string id = controller.getAssignmentId("Homework 1");

    // invalid argument since title is empty
    ASSERT_THROW(controller.editTitle(id, ""), std::invalid_argument);
}

TEST_F(AssignmentControllerTest, EditDueDateInvalid) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    std::string id = controller.getAssignmentId("Homework 1");

    // invalid argument since due date doesn't exist
    ASSERT_THROW(controller.editDueDate(id, std::chrono::year_month_day{2026y/3/33}), std::invalid_argument);
}

TEST_F(AssignmentControllerTest, EditGradeInvalidLow) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    std::string id = controller.getAssignmentId("Homework 1");

    // out of range since grade is below 0
    ASSERT_THROW(controller.editGrade(id, -10.0f), std::out_of_range);
}

TEST_F(AssignmentControllerTest, EditGradeInvalidHigh) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    std::string id = controller.getAssignmentId("Homework 1");

    // out of range since grade is over 100
    ASSERT_THROW(controller.editGrade(id, 110.0f), std::out_of_range);
}

TEST_F(AssignmentControllerTest, EditGradeBoundaryLow) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    std::string id = controller.getAssignmentId("Homework 1");

    controller.editGrade(id, 0.0f);

    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 0.0f);
}

TEST_F(AssignmentControllerTest, EditGradeBoundaryHigh) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    std::string id = controller.getAssignmentId("Homework 1");

    controller.editGrade(id, 100.0f);
    
    const Assignment& selectedAssignment = controller.findAssignment("Homework 1");
    ASSERT_FLOAT_EQ(selectedAssignment.getGrade(), 100.0f);
}

TEST_F(AssignmentControllerTest, RemoveAssignmentNotFound) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

    // out of range since assignment cannot be found
    ASSERT_THROW(controller.removeAssignment("Homework 4"), std::out_of_range);
}

TEST_F(AssignmentControllerTest, FindAssignmentConstNotFound) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

    // out of range error since assignment cannot be found
    const AssignmentController& constController = controller;
    ASSERT_THROW(constController.findAssignment("Homework 4"), std::out_of_range);
}

TEST_F(AssignmentControllerTest, FindAssignmentNonConstNotFound) {
    controller.addAssignment("Homework 1", "", std::chrono::year_month_day{2026y/1/12}, true, 90.0f);
    controller.addAssignment("Homework 2", "Functions and variables", std::chrono::year_month_day{2026y/1/22}, false, 0.0f);

    // out of range error since assignment cannot be found
    ASSERT_THROW(controller.findAssignment("Homework 4"), std::out_of_range);
}