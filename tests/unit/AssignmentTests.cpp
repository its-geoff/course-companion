#include <gtest/gtest.h>
#include <sstream>
#include "Assignment.hpp"

using namespace std::chrono_literals;

// test fixture for class Assignment
class AssignmentTest : public testing::Test {
    protected:
        Assignment assignment1{
            "Homework 3", 
            "Focus on variables and strings.", 
            std::chrono::year_month_day{2025y/11/20},
            true,
            95.18f
        };
};

// ====================================
// GETTER SMOKE TESTS
// ====================================

TEST_F(AssignmentTest, TitleGetter) {
    ASSERT_EQ(assignment1.getTitle(), "Homework 3");
}

TEST_F(AssignmentTest, DescriptionGetter) {
    ASSERT_EQ(assignment1.getDescription(), "Focus on variables and strings.");
}

TEST_F(AssignmentTest, DueDateGetter) {
    ASSERT_EQ(assignment1.getDueDate(), std::chrono::year_month_day{2025y/11/20});
}

TEST_F(AssignmentTest, CompletedGetter) {
    ASSERT_EQ(assignment1.getCompleted(), true);
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

TEST_F(AssignmentTest, DueDateSetter) {
    assignment1.setDueDate(std::chrono::year_month_day{2025y/11/22});
    ASSERT_EQ(assignment1.getDueDate(), std::chrono::year_month_day{2025y/11/22});
}

TEST_F(AssignmentTest, CompletedSetter) {
    assignment1.setCompleted(false);
    ASSERT_EQ(assignment1.getCompleted(), false);
}

TEST_F(AssignmentTest, GradeSetter) {
    assignment1.setGrade(96.20f);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 96.20f);
}

// ====================================
// INITIALIZATION SMOKE TESTS
// ====================================

// initializations without description defined
TEST_F(AssignmentTest, OneParamInitialization) {
    Assignment assignment2{"Homework 1"};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
}

TEST_F(AssignmentTest, TwoParamInitialization) {
    Assignment assignment2{"Homework 1", std::chrono::year_month_day{2025y/10/31}};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
}

TEST_F(AssignmentTest, ThreeParamInitialization) {
    Assignment assignment2{"Homework 1", std::chrono::year_month_day{2025y/10/31}, false};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
    ASSERT_EQ(assignment2.getCompleted(), false);
}

TEST_F(AssignmentTest, FourParamInitialization) {
    Assignment assignment2{"Homework 1", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
    ASSERT_EQ(assignment2.getCompleted(), false);
    ASSERT_EQ(assignment2.getGrade(), 90.50f);
}

// initializations with description defined
TEST_F(AssignmentTest, TwoParamDescInitialization) {
    Assignment assignment2{"Homework 1", "Focus on lexical analysis."};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
}

TEST_F(AssignmentTest, ThreeParamDescInitialization) {
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", std::chrono::year_month_day{2025y/10/31}};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
}

TEST_F(AssignmentTest, FourParamDescInitialization) {
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", std::chrono::year_month_day{2025y/10/31}, false};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
    ASSERT_EQ(assignment2.getCompleted(), false);
}

TEST_F(AssignmentTest, FiveParamDescInitialization) {
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
    ASSERT_EQ(assignment2.getCompleted(), false);
    ASSERT_EQ(assignment2.getGrade(), 90.50f);
}

// ====================================
// FUNCTION SMOKE TESTS
// ====================================

TEST_F(AssignmentTest, PrintAssignmentInfo) {
    std::stringstream ss;

    assignment1.printAssignmentInfo(ss);
    ASSERT_EQ(ss.str(), "===========================================================\nAssignment Title: Homework 3\nDescription: Focus on variables and strings.\n"
                        "Due Date: 2025-11-20\nCompleted? Yes\nGrade: 95.18\n===========================================================\n");
}

TEST_F(AssignmentTest, CompletedString) {
    Assignment assignment2{"Homework 1", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};
    ASSERT_EQ(assignment1.completedString(assignment1.getCompleted()), "Yes");
    ASSERT_EQ(assignment2.completedString(assignment2.getCompleted()), "No");
}

// ====================================
// GETTER EDGE CASES
// ====================================

// empty parameters
TEST_F(AssignmentTest, DescriptionGetterEmpty) {
    Assignment assignment2{"Homework 1", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};
    ASSERT_EQ(assignment2.getDescription(), "");
}

TEST_F(AssignmentTest, DueDateGetterEmpty) {
    Assignment assignment2{"Homework 1"};
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::time_point_cast<std::chrono::days>(now);    // set to today
    ASSERT_EQ(assignment2.getDueDate(), today);
}

TEST_F(AssignmentTest, CompletedGetterEmpty) {
    Assignment assignment2{"Homework 1", std::chrono::year_month_day{2025y/10/31}};
    ASSERT_EQ(assignment2.getCompleted(), false);
}

TEST_F(AssignmentTest, GradeGetterEmpty) {
    Assignment assignment2{"Homework 1", std::chrono::year_month_day{2025y/10/31}, false};
    ASSERT_FLOAT_EQ(assignment2.getGrade(), 0.0f);
}

// invalid parameters
TEST_F(AssignmentTest, DueDateGetterInvalid) {
    // throw invalid argument since date does not exist
    ASSERT_THROW((Assignment{"Homework 1", std::chrono::year_month_day{2025y/2/30}, false, 90.50f}), std::invalid_argument);
}

TEST_F(AssignmentTest, GradeGetterInvalidLow) {
    // throw out of range error since number is not in range 0 to 100
    ASSERT_THROW((Assignment{"Homework 1", std::chrono::year_month_day{2025y/10/31}, false, -20.24}), std::out_of_range);
}

TEST_F(AssignmentTest, GradeGetterInvalidHigh) {
    // throw out of range error since number is not in range 0 to 100
    ASSERT_THROW((Assignment{"Homework 1", std::chrono::year_month_day{2025y/10/31}, false, 200.24}), std::out_of_range);
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

TEST_F(AssignmentTest, GradeSetterInvalidLow) {
    // throw out of range since input is not in range 0 to 100
    ASSERT_THROW(assignment1.setGrade(-20.24f), std::out_of_range);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 95.18f);
}

TEST_F(AssignmentTest, GradeSetterInvalidHigh) {
    // throw out of range since input is not in range 0 to 100
    ASSERT_THROW(assignment1.setGrade(200.24f), std::out_of_range);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 95.18f);
}

TEST_F(AssignmentTest, GradeSetterBoundaryLow) {
    assignment1.setGrade(0.0f);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 0.0f);
}

TEST_F(AssignmentTest, GradeSetterBoundaryHigh) {
    assignment1.setGrade(100.0);
    ASSERT_FLOAT_EQ(assignment1.getGrade(), 100.0f);
}

// ====================================
// INITIALIZATION EDGE CASES
// ====================================

// invalid initializations without description defined
TEST_F(AssignmentTest, OneParamInitializationInvalid) {
    // throw invalid argument since input is empty
    ASSERT_THROW((Assignment{""}), std::invalid_argument);
}

TEST_F(AssignmentTest, TwoParamInitializationInvalid) {
    // throw invalid argument since date does not exist 
    ASSERT_THROW((Assignment{"Homework 1", std::chrono::year_month_day{2025y/2/30}}), std::invalid_argument);
}

TEST_F(AssignmentTest, FourParamInitializationInvalidLow) {
    // throw out of range since input is not in range 0 to 100
    ASSERT_THROW((Assignment{"Homework 1", std::chrono::year_month_day{2025y/10/31}, false, -20.24f}), std::out_of_range);
}

TEST_F(AssignmentTest, FourParamInitializationInvalidHigh) {
    // throw out of range since input is not in range 0 to 100
    ASSERT_THROW((Assignment{"Homework 1", std::chrono::year_month_day{2025y/10/31}, false, 200.24f}), std::out_of_range);
}

// invalid initializations with description defined
TEST_F(AssignmentTest, TwoParamDescInitializationInvalid) {
    // throw invalid argument since title is empty
    ASSERT_THROW((Assignment{"", "Focus on lexical analysis."}), std::invalid_argument);
}

TEST_F(AssignmentTest, ThreeParamDescInitializationInvalid) {
    // throw invalid argument since date does not exist
    ASSERT_THROW((Assignment{"Homework 1", "Focus on lexical analysis.", std::chrono::year_month_day{2025y/2/30}}), std::invalid_argument);
}

TEST_F(AssignmentTest, FiveParamDescInitializationInvalidLow) {
    // throw out of range since input is not in range 0 to 100
    ASSERT_THROW((Assignment{"Homework 1", "Focus on lexical analysis.", std::chrono::year_month_day{2025y/10/31}, false, -20.24f}), std::out_of_range);
}

TEST_F(AssignmentTest, FiveParamDescInitializationInvalidHigh) {
    // throw out of range since input is not in range 0 to 100
    ASSERT_THROW((Assignment{"Homework 1", "Focus on lexical analysis.", std::chrono::year_month_day{2025y/10/31}, false, 200.24f}), std::out_of_range);
}

// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST_F(AssignmentTest, PrintAssignmentInfoPartial) {
    std::stringstream ss;
    Assignment assignment2{"Homework 1", std::chrono::year_month_day{2025y/10/31}};

    assignment2.printAssignmentInfo(ss);
    ASSERT_EQ(ss.str(), "===========================================================\nAssignment Title: Homework 1\n"
                        "Due Date: 2025-10-31\nCompleted? No\nGrade: 0\n===========================================================\n");
}

TEST_F(AssignmentTest, PrintAssignmentInfoDescPartial) {
    std::stringstream ss;
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", std::chrono::year_month_day{2025y/10/31}};

    assignment2.printAssignmentInfo(ss);
    ASSERT_EQ(ss.str(), "===========================================================\nAssignment Title: Homework 1\nDescription: Focus on lexical analysis.\n"
                        "Due Date: 2025-10-31\nCompleted? No\nGrade: 0\n===========================================================\n");
}

TEST_F(AssignmentTest, PrintAssignmentInfoInteger) {
    std::stringstream ss;
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", std::chrono::year_month_day{2025y/10/31}, true, 75};

    assignment2.printAssignmentInfo(ss);
    ASSERT_EQ(ss.str(), "===========================================================\nAssignment Title: Homework 1\nDescription: Focus on lexical analysis.\n"
                        "Due Date: 2025-10-31\nCompleted? Yes\nGrade: 75\n===========================================================\n");
}

TEST_F(AssignmentTest, ReadOptionalStringEmpty) {
    std::stringstream ss1("");
    std::stringstream ss2("  ");

    ASSERT_EQ(assignment1.readOptionalString(ss1), std::nullopt);
    ASSERT_EQ(assignment1.readOptionalString(ss2), std::nullopt);
}

TEST_F(AssignmentTest, ReadOptionalDateEmpty) {
    std::stringstream ss1("");
    std::stringstream ss2("  ");

    ASSERT_EQ(assignment1.readOptionalDate(ss1), std::nullopt);
    ASSERT_EQ(assignment1.readOptionalDate(ss2), std::nullopt);
}

TEST_F(AssignmentTest, ReadOptionalBoolEmpty) {
    std::stringstream ss1("");
    std::stringstream ss2("  ");

    ASSERT_EQ(assignment1.readOptionalBool(ss1), std::nullopt);
    ASSERT_EQ(assignment1.readOptionalBool(ss2), std::nullopt);
}

TEST_F(AssignmentTest, ReadOptionalFloatEmpty) {
    std::stringstream ss1("");
    std::stringstream ss2("  ");

    ASSERT_EQ(assignment1.readOptionalFloat(ss1), std::nullopt);
    ASSERT_EQ(assignment1.readOptionalFloat(ss2), std::nullopt);
}

TEST_F(AssignmentTest, ReadOptionalBoolAlternatives) {
    std::stringstream ss_y(
        "yes\n"
        "y\n"
        "true\n"
        "1\n"
    );
    std::stringstream ss_n(
        "no\n"
        "n\n"
        "false\n"
        "0\n"
    );

    // checks each line separately and asserts true or false, based on conversion
    ASSERT_EQ(assignment1.readOptionalBool(ss_y).value(), true);
    ASSERT_EQ(assignment1.readOptionalBool(ss_y).value(), true);
    ASSERT_EQ(assignment1.readOptionalBool(ss_y).value(), true);
    ASSERT_EQ(assignment1.readOptionalBool(ss_y).value(), true);
    ASSERT_EQ(assignment1.readOptionalBool(ss_n).value(), false);
    ASSERT_EQ(assignment1.readOptionalBool(ss_n).value(), false);
    ASSERT_EQ(assignment1.readOptionalBool(ss_n).value(), false);
    ASSERT_EQ(assignment1.readOptionalBool(ss_n).value(), false);
}

// ====================================
// CLASS USE CASES
// ====================================

// add assignment (partial info)
// print assignment info for both 1 and 2
// change some assignment info for 1 (setters)
// retrieve some old and some new info (getters)
// remove assignment

// ====================================
// MAIN FUNCTION
// ====================================
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}