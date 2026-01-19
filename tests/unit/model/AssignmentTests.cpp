#include <gtest/gtest.h>
#include <sstream>      // diverts output from terminal to separate stream
#include <regex>        // regular expression matching for UUIDs
#include "model/Assignment.hpp"
#include "utils/utils.hpp"

using namespace std::chrono_literals;

// test fixture for class Assignment
class AssignmentTest : public testing::Test {
    protected:
        Assignment assignment1{
            "Homework 3", 
            "Focus on variables and strings.", 
            "Homework", 
            std::chrono::year_month_day{2025y/11/20},
            true,
            95.18f
        };
};

// ====================================
// GETTER SMOKE TESTS
// ====================================

TEST_F(AssignmentTest, IdGetter) {
    // ensure ID is not empty
    ASSERT_FALSE(utils::isOnlyWhitespace(assignment1.getId()));
}

TEST_F(AssignmentTest, TitleGetter) {
    ASSERT_EQ(assignment1.getTitle(), "Homework 3");
}

TEST_F(AssignmentTest, DescriptionGetter) {
    ASSERT_EQ(assignment1.getDescription(), "Focus on variables and strings.");
}

TEST_F(AssignmentTest, CategoryGetter) {
    ASSERT_EQ(assignment1.getCategory(), "Homework");
}

TEST_F(AssignmentTest, DueDateGetter) {
    ASSERT_EQ(assignment1.getDueDate(), std::chrono::year_month_day{2025y/11/20});
}

TEST_F(AssignmentTest, CompletedGetter) {
    ASSERT_TRUE(assignment1.getCompleted());
}

TEST_F(AssignmentTest, GradeGetter) {
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 95.18f);
}

// ====================================
// SETTER SMOKE TESTS
// ====================================

TEST_F(AssignmentTest, TitleSetter) {
    assignment1.setTitle("Homework 2");
    ASSERT_EQ(assignment1.getTitle(), "Homework 2");
}

TEST_F(AssignmentTest, DescriptionSetter) {
    assignment1.setDescription("Focus on order of operations.");
    ASSERT_EQ(assignment1.getDescription(), "Focus on order of operations.");
}

TEST_F(AssignmentTest, CategorySetter) {
    assignment1.setCategory("Midterm");
    ASSERT_EQ(assignment1.getCategory(), "Midterm");
}

TEST_F(AssignmentTest, DueDateSetter) {
    assignment1.setDueDate(std::chrono::year_month_day{2025y/11/22});
    ASSERT_EQ(assignment1.getDueDate(), std::chrono::year_month_day{2025y/11/22});
}

TEST_F(AssignmentTest, CompletedSetter) {
    assignment1.setCompleted(false);
    ASSERT_FALSE(assignment1.getCompleted());
}

TEST_F(AssignmentTest, GradeSetterPercentage) {
    assignment1.setGrade(96.20f);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 96.20f);
}

TEST_F(AssignmentTest, GradeSetterPoints) {
    assignment1.setGrade(18, 20);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 90.0f);
}

// ====================================
// INITIALIZATION SMOKE TESTS
// ====================================

// initializations without description defined
TEST_F(AssignmentTest, TwoParamInitialization) {
    Assignment assignment2{"Homework 1", "", "Homework", {}};
    ASSERT_FALSE(utils::isOnlyWhitespace(assignment2.getId()));
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getCategory(), "Homework");
}

TEST_F(AssignmentTest, ThreeParamInitialization) {
    Assignment assignment2{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/10/31}};
    ASSERT_FALSE(utils::isOnlyWhitespace(assignment2.getId()));
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getCategory(), "Homework");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
}

TEST_F(AssignmentTest, FourParamInitialization) {
    Assignment assignment2{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/10/31}, false};
    ASSERT_FALSE(utils::isOnlyWhitespace(assignment2.getId()));
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getCategory(), "Homework");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
    ASSERT_FALSE(assignment2.getCompleted());
}

TEST_F(AssignmentTest, FiveParamInitialization) {
    Assignment assignment2{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};
    ASSERT_FALSE(utils::isOnlyWhitespace(assignment2.getId()));
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getCategory(), "Homework");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
    ASSERT_FALSE(assignment2.getCompleted());
    ASSERT_EQ(assignment2.getGrade(), 90.50f);
}

// initializations with description defined
TEST_F(AssignmentTest, ThreeParamDescInitialization) {
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", "Homework", {}};
    ASSERT_FALSE(utils::isOnlyWhitespace(assignment2.getId()));
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getCategory(), "Homework");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
}

TEST_F(AssignmentTest, FourParamDescInitialization) {
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", "Homework", std::chrono::year_month_day{2025y/10/31}};
    ASSERT_FALSE(utils::isOnlyWhitespace(assignment2.getId()));
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getCategory(), "Homework");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
}

TEST_F(AssignmentTest, FiveParamDescInitialization) {
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", "Homework", std::chrono::year_month_day{2025y/10/31}, false};
    ASSERT_FALSE(utils::isOnlyWhitespace(assignment2.getId()));
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getCategory(), "Homework");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
    ASSERT_FALSE(assignment2.getCompleted());
}

TEST_F(AssignmentTest, SixParamDescInitialization) {
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", "Homework", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};
    ASSERT_FALSE(utils::isOnlyWhitespace(assignment2.getId()));
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getCategory(), "Homework");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
    ASSERT_FALSE(assignment2.getCompleted());
    ASSERT_EQ(assignment2.getGrade(), 90.50f);
}

// ====================================
// FUNCTION SMOKE TESTS
// ====================================

TEST_F(AssignmentTest, PrintAssignmentInfo) {
    std::stringstream ss;

    assignment1.printAssignmentInfo(ss);
    std::string output = ss.str();

    // find and replace the UUID part
    std::regex uuidRegex("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    output = std::regex_replace(output, uuidRegex, "<UUID>");

    ASSERT_EQ(output, "ID: <UUID>\nAssignment: Homework 3\nDescription: Focus on variables and strings.\n"
                "Category: Homework\nDue Date: 2025-11-20\nCompleted? Yes\nGrade: 95.18\n");
}

TEST_F(AssignmentTest, OverloadedEquals) {
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", "Homework", std::chrono::year_month_day{2025y/10/31}, true, 75};
    Assignment assignment3{"Homework 3", "Focus on variables and strings.", "Homework", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};
    Assignment assignment4 = assignment1;

    ASSERT_FALSE(assignment1 == assignment2);
    ASSERT_FALSE(assignment1 == assignment3);
    ASSERT_TRUE(assignment1 == assignment4);
}

// ====================================
// GETTER EDGE CASES
// ====================================

// empty parameters
TEST_F(AssignmentTest, DescriptionGetterEmpty) {
    Assignment assignment2{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};
    ASSERT_EQ(assignment2.getDescription(), "");
}

TEST_F(AssignmentTest, DueDateGetterEmpty) {
    Assignment assignment2{"Homework 1", "", "Homework", {}};
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::time_point_cast<std::chrono::days>(now);    // set to today
    ASSERT_EQ(assignment2.getDueDate(), today);
}

TEST_F(AssignmentTest, CompletedGetterEmpty) {
    Assignment assignment2{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/10/31}};
    ASSERT_FALSE(assignment2.getCompleted());
}

TEST_F(AssignmentTest, GradeGetterEmpty) {
    Assignment assignment2{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/10/31}, false};
    ASSERT_FLOAT_EQ(assignment2.getGrade(), 0.0f);
}

// ====================================
// SETTER EDGE CASES
// ====================================

TEST_F(AssignmentTest, TitleSetterInvalid) {
    // throw invalid argument since title is empty
    ASSERT_THROW(assignment1.setTitle(""), std::invalid_argument);
    ASSERT_EQ(assignment1.getTitle(), "Homework 3");
}

TEST_F(AssignmentTest, TitleSetterWhitespaceInvalid) {
    // throw invalid argument since title is whitespace
    ASSERT_THROW(assignment1.setTitle(" "), std::invalid_argument);
    ASSERT_EQ(assignment1.getTitle(), "Homework 3");
}

TEST_F(AssignmentTest, DueDateSetterInvalid) {
    // throw invalid argument since date does not exist
    ASSERT_THROW(assignment1.setDueDate(std::chrono::year_month_day{2025y/2/30}), std::invalid_argument);
    ASSERT_EQ(assignment1.getDueDate(), std::chrono::year_month_day{2025y/11/20});
}

TEST_F(AssignmentTest, GradeSetterPercentageInvalidLow) {
    // throw out of range since input is not in range 0 to 150
    ASSERT_THROW(assignment1.setGrade(-20.24f), std::out_of_range);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 95.18f);
}

TEST_F(AssignmentTest, GradeSetterPercentageInvalidHigh) {
    // throw out of range since input is not in range 0 to 150
    ASSERT_THROW(assignment1.setGrade(200.24f), std::out_of_range);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 95.18f);
}

TEST_F(AssignmentTest, GradeSetterPercentageBoundaryLow) {
    assignment1.setGrade(0.0f);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 0.0f);
}

TEST_F(AssignmentTest, GradeSetterPercentageBoundaryHigh) {
    assignment1.setGrade(100.0f);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 100.0f);
}

TEST_F(AssignmentTest, GradeSetterPointsInvalidLow) {
    // throw out of range since input is not in range 0 to 150
    ASSERT_THROW(assignment1.setGrade(-3, 20), std::out_of_range);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 95.18f);
}

TEST_F(AssignmentTest, GradeSetterPointsInvalidHigh) {
    // throw out of range since input is not in range 0 to 150
    ASSERT_THROW(assignment1.setGrade(40, 20), std::out_of_range);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 95.18f);
}

TEST_F(AssignmentTest, GradeSetterPointsBoundaryLow) {
    assignment1.setGrade(0, 20);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 0.0f);
}

TEST_F(AssignmentTest, GradeSetterPointsBoundaryHigh) {
    assignment1.setGrade(20, 20);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 100.0f);
}

TEST_F(AssignmentTest, GradeSetterPointsNegativeTotal) {
    // throw invalid argument since total points is negative
    ASSERT_THROW(assignment1.setGrade(20, -20), std::invalid_argument);
}

TEST_F(AssignmentTest, GradeSetterPointsZeroTotal) {
    // throw invalid argument since total points is zero
    ASSERT_THROW(assignment1.setGrade(20, 0), std::invalid_argument);
}

// ====================================
// INITIALIZATION EDGE CASES
// ====================================

// invalid initializations without description defined
TEST_F(AssignmentTest, TwoParamInitializationInvalidTitle) {
    // throw invalid argument since title is empty
    ASSERT_THROW((Assignment{"", "", "Homework", {}}), std::invalid_argument);
}

TEST_F(AssignmentTest, TwoParamInitialiationInvalidCategory) {
    // throw invalid argument since category is empty
    ASSERT_THROW((Assignment{"Homework 1", "", "", {}}), std::invalid_argument);
}

TEST_F(AssignmentTest, ThreeParamInitializationInvalidDate) {
    // throw invalid argument since date does not exist 
    ASSERT_THROW((Assignment{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/2/30}}), std::invalid_argument);
}

TEST_F(AssignmentTest, FiveParamInitializationInvalidGradeLow) {
    // throw out of range since input is not in range 0 to 150
    ASSERT_THROW((Assignment{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/10/31}, false, -20.24f}), std::out_of_range);
}

TEST_F(AssignmentTest, FiveParamInitializationInvalidGradeHigh) {
    // throw out of range since input is not in range 0 to 150
    ASSERT_THROW((Assignment{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/10/31}, false, 200.24f}), std::out_of_range);
}

// invalid initializations with description defined
TEST_F(AssignmentTest, ThreeParamDescInitializationInvalidTitle) {
    // throw invalid argument since title is empty
    ASSERT_THROW((Assignment{"", "Focus on lexical analysis.", "Homework", {}}), std::invalid_argument);
}

TEST_F(AssignmentTest, ThreeParamDescInitializationInvalidCategory) {
    // throw invalid argument since category is empty
    ASSERT_THROW((Assignment{"Homework 1", "Focus on lexical analysis.", "", {}}), std::invalid_argument);
}

TEST_F(AssignmentTest, FourParamDescInitializationInvalidDate) {
    // throw invalid argument since date does not exist
    ASSERT_THROW((Assignment{"Homework 1", "Focus on lexical analysis.", "Homework", std::chrono::year_month_day{2025y/2/30}}), std::invalid_argument);
}

TEST_F(AssignmentTest, SixParamDescInitializationInvalidGradeLow) {
    // throw out of range since input is not in range 0 to 150
    ASSERT_THROW((Assignment{"Homework 1", "Focus on lexical analysis.", "Homework", std::chrono::year_month_day{2025y/10/31}, false, -20.24f}), std::out_of_range);
}

TEST_F(AssignmentTest, SixParamDescInitializationInvalidGradeHigh) {
    // throw out of range since input is not in range 0 to 150
    ASSERT_THROW((Assignment{"Homework 1", "Focus on lexical analysis.", "Homework", std::chrono::year_month_day{2025y/10/31}, false, 200.24f}), std::out_of_range);
}

// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST_F(AssignmentTest, PrintAssignmentInfoPartial) {
    std::stringstream ss;
    Assignment assignment2{"Homework 1", "", "Homework", std::chrono::year_month_day{2025y/10/31}};
    assignment2.printAssignmentInfo(ss);
    std::string output = ss.str();

    // find and replace the UUID part
    std::regex uuidRegex("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    output = std::regex_replace(output, uuidRegex, "<UUID>");

    ASSERT_EQ(output, "ID: <UUID>\nAssignment: Homework 1\nCategory: Homework\n"
                "Due Date: 2025-10-31\nCompleted? No\nGrade: 0\n");
}

TEST_F(AssignmentTest, PrintAssignmentInfoDescPartial) {
    std::stringstream ss;
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", "Homework", std::chrono::year_month_day{2025y/10/31}};
    assignment2.printAssignmentInfo(ss);
    std::string output = ss.str();

    // find and replace the UUID part
    std::regex uuidRegex("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    output = std::regex_replace(output, uuidRegex, "<UUID>");

    ASSERT_EQ(output, "ID: <UUID>\nAssignment: Homework 1\nDescription: Focus on lexical analysis.\n"
                "Category: Homework\nDue Date: 2025-10-31\nCompleted? No\nGrade: 0\n");
}

TEST_F(AssignmentTest, PrintAssignmentInfoInteger) {
    std::stringstream ss;
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", "Homework", std::chrono::year_month_day{2025y/10/31}, true, 75};
    assignment2.printAssignmentInfo(ss);
    std::string output = ss.str();

    // find and replace the UUID part
    std::regex uuidRegex("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    output = std::regex_replace(output, uuidRegex, "<UUID>");

    ASSERT_EQ(output, "ID: <UUID>\nAssignment: Homework 1\nDescription: Focus on lexical analysis.\n"
                "Category: Homework\nDue Date: 2025-10-31\nCompleted? Yes\nGrade: 75\n");
}

TEST_F(AssignmentTest, OverloadedEqualsSameTitleDifferentParams) {
    Assignment assignment2{"Homework 1", "Focus on variables and strings.", "Homework", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};
    Assignment assignment3{"Homework 3", "Focus on functions.", "Homework", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};
    Assignment assignment4{"Homework 3", "Focus on variables and strings.", "Homework", std::chrono::year_month_day{2025y/11/19}, true, 95.18f};
    Assignment assignment5{"Homework 3", "Focus on variables and strings.", "Homework", std::chrono::year_month_day{2025y/11/20}, false, 92.71f};

    ASSERT_FALSE(assignment1 == assignment2);
    ASSERT_FALSE(assignment1 == assignment3);
    ASSERT_FALSE(assignment1 == assignment4);
    ASSERT_FALSE(assignment1 == assignment5);
}

TEST_F(AssignmentTest, OverloadedEqualsSameParamsDifferentId) {
    Assignment assignment2{"Homework 1", "Focus on variables and strings.", "Homework", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};
    Assignment assignment3{"Homework 1", "Focus on variables and strings.", "Homework", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};

    ASSERT_FALSE(assignment2 == assignment3);
}