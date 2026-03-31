#include <gtest/gtest.h>
#include <chrono>
#include <string>
#include "model/Assignment.hpp"
#include "utils/utils.hpp"

using namespace std::chrono_literals;

// test fixture for class AssignmentRepository
class AssignmentRepositoryTest : public testing::Test {
    protected:
        const std::string validId       = "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
        const std::string validTitle    = "Homework 3";
        const std::string validDesc     = "Focus on variables and strings.";
        const std::string validCategory = "Homework";
        const std::chrono::year_month_day validDueDate{2025y/11/20};
};

// ====================================
// SMOKE TESTS
// ====================================

TEST_F(AssignmentRepositoryTest, FromRowAllFields) {
    Assignment assignment = Assignment::fromRow(validId, validTitle, validDesc,
        validCategory, validDueDate, true, 95.18f);

    ASSERT_EQ(assignment.getId(),          validId);
    ASSERT_EQ(assignment.getTitle(),       validTitle);
    ASSERT_EQ(assignment.getDescription(), validDesc);
    ASSERT_EQ(assignment.getCategory(),    validCategory);
    ASSERT_EQ(assignment.getDueDate(),     validDueDate);
    ASSERT_TRUE(assignment.getCompleted());
    ASSERT_FLOAT_EQ(assignment.getGrade(), 95.18f);
}

TEST_F(AssignmentRepositoryTest, FromRowPreservesId) {
    Assignment assignment = Assignment::fromRow(validId, validTitle, validDesc,
        validCategory, validDueDate, false, 0.0f);

    ASSERT_EQ(assignment.getId(), validId);
}

TEST_F(AssignmentRepositoryTest, FromRowCompletedTrue) {
    Assignment assignment = Assignment::fromRow(validId, validTitle, validDesc,
        validCategory, validDueDate, true, 95.18f);

    ASSERT_TRUE(assignment.getCompleted());
}

TEST_F(AssignmentRepositoryTest, FromRowCompletedFalse) {
    Assignment assignment = Assignment::fromRow(validId, validTitle, validDesc,
        validCategory, validDueDate, false, 0.0f);

    ASSERT_FALSE(assignment.getCompleted());
}

TEST_F(AssignmentRepositoryTest, FromRowEmptyDescription) {
    Assignment assignment = Assignment::fromRow(validId, validTitle, "",
        validCategory, validDueDate, false, 0.0f);

    ASSERT_EQ(assignment.getDescription(), "");
}

TEST_F(AssignmentRepositoryTest, FromRowTwoAssignmentsRetainSeparateIds) {
    std::string id1 = "550e8400-e29b-41d4-a716-446655440000";
    std::string id2 = "550e8400-e29b-41d4-a716-446655440001";

    Assignment assignment1 = Assignment::fromRow(id1, "Homework 3", validDesc,
        validCategory, validDueDate, true, 95.18f);
    Assignment assignment2 = Assignment::fromRow(id2, "Midterm", "",
        "Midterm", std::chrono::year_month_day{2025y/10/15}, true, 88.0f);

    ASSERT_EQ(assignment1.getId(), id1);
    ASSERT_EQ(assignment2.getId(), id2);
}

TEST_F(AssignmentRepositoryTest, FromRowDoesNotEqualNewAssignment) {
    // a fromRow Assignment and a newly constructed Assignment with the same parameters should not be equal
    Assignment fromRowAssignment = Assignment::fromRow(validId, validTitle, validDesc,
        validCategory, validDueDate, true, 95.18f);
    Assignment newAssignment{validTitle, validDesc, validCategory, validDueDate, true, 95.18f};

    ASSERT_NE(fromRowAssignment, newAssignment);
}

// ====================================
// EDGE CASES
// ====================================

TEST_F(AssignmentRepositoryTest, FromRowEmptyTitle) {
    // throw invalid argument since title is empty
    ASSERT_THROW(Assignment::fromRow(validId, "", validDesc,
        validCategory, validDueDate, false, 0.0f), std::invalid_argument);
}

TEST_F(AssignmentRepositoryTest, FromRowWhitespaceTitle) {
    // throw invalid argument since title is whitespace
    ASSERT_THROW(Assignment::fromRow(validId, "   ", validDesc,
        validCategory, validDueDate, false, 0.0f), std::invalid_argument);
}

TEST_F(AssignmentRepositoryTest, FromRowEmptyCategory) {
    // throw invalid argument since category is empty
    ASSERT_THROW(Assignment::fromRow(validId, validTitle, validDesc,
        "", validDueDate, false, 0.0f), std::invalid_argument);
}

TEST_F(AssignmentRepositoryTest, FromRowEmptyDueDate) {
    // throw invalid argument since due date is empty
    ASSERT_THROW(Assignment::fromRow(validId, validTitle, validDesc,
        validCategory, std::chrono::year_month_day{}, false, 0.0f), std::invalid_argument);
}

TEST_F(AssignmentRepositoryTest, FromRowInvalidDueDate) {
    // throw invalid argument since due date does not exist
    ASSERT_THROW(Assignment::fromRow(validId, validTitle, validDesc,
        validCategory, std::chrono::year_month_day{2025y/2/30}, false, 0.0f), std::invalid_argument);
}

TEST_F(AssignmentRepositoryTest, FromRowGradeTooLow) {
    // throw out of range since grade is below 0
    ASSERT_THROW(Assignment::fromRow(validId, validTitle, validDesc,
        validCategory, validDueDate, true, -1.0f), std::out_of_range);
}

TEST_F(AssignmentRepositoryTest, FromRowGradeTooHigh) {
    // throw out of range since grade is above 150
    ASSERT_THROW(Assignment::fromRow(validId, validTitle, validDesc,
        validCategory, validDueDate, true, 200.0f), std::out_of_range);
}

TEST_F(AssignmentRepositoryTest, FromRowCompletedFalseGradeIgnored) {
    // grade is zeroed when completed is false
    Assignment assignment = Assignment::fromRow(validId, validTitle, validDesc,
        validCategory, validDueDate, false, 90.0f);

    ASSERT_FLOAT_EQ(assignment.getGrade(), 0.0f);
}

TEST_F(AssignmentRepositoryTest, FromRowGradeBoundaryLow) {
    Assignment assignment = Assignment::fromRow(validId, validTitle, validDesc,
        validCategory, validDueDate, true, 0.0f);

    ASSERT_FLOAT_EQ(assignment.getGrade(), 0.0f);
}

TEST_F(AssignmentRepositoryTest, FromRowGradeBoundaryHigh) {
    Assignment assignment = Assignment::fromRow(validId, validTitle, validDesc,
        validCategory, validDueDate, true, 150.0f);

    ASSERT_FLOAT_EQ(assignment.getGrade(), 150.0f);
}

TEST_F(AssignmentRepositoryTest, FromRowLeapYearDueDate) {
    Assignment assignment = Assignment::fromRow(validId, validTitle, validDesc,
        validCategory, std::chrono::year_month_day{2024y/2/29}, false, 0.0f);

    ASSERT_EQ(assignment.getDueDate(), (std::chrono::year_month_day{2024y/2/29}));
}