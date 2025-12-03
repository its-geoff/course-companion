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
            std::chrono::sys_days{2025y/11/20},
            true,
            95.18
        };
};

// test class to redirect output
class CoutRedirect {
    public:
        CoutRedirect(std::ostream& newStream)
            : oldBuf(std::cout.rdbuf(newStream.rdbuf()))
        {}

        ~CoutRedirect() {
            std::cout.rdbuf(oldBuf);
        }

    private:
        std::streambuf* oldBuf;
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
    ASSERT_EQ(assignment1.getDueDate(), std::chrono::sys_days{2025y/11/20});
}

TEST_F(AssignmentTest, CompletedGetter) {
    ASSERT_EQ(assignment1.getCompleted(), true);
}

TEST_F(AssignmentTest, GradeGetter) {
    ASSERT_NEAR(assignment1.getGrade(), 95.18, 1e-5);
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
    assignment1.setDueDate(std::chrono::sys_days{2025y/11/22});
    ASSERT_EQ(assignment1.getDueDate(), std::chrono::sys_days{2025y/11/22});
}

TEST_F(AssignmentTest, CompletedSetter) {
    assignment1.setCompleted(false);
    ASSERT_EQ(assignment1.getCompleted(), false);
}

TEST_F(AssignmentTest, GradeSetter) {
    assignment1.setGrade(96.20);
    ASSERT_NEAR(assignment1.getGrade(), 96.20, 1e-5);
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
    Assignment assignment2{"Homework 1", std::chrono::sys_days{2025y/10/31}};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::sys_days{2025y/10/31});
}

TEST_F(AssignmentTest, ThreeParamInitialization) {
    Assignment assignment2{"Homework 1", std::chrono::sys_days{2025y/10/31}, false};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::sys_days{2025y/10/31});
    ASSERT_EQ(assignment2.getCompleted(), false);
}

TEST_F(AssignmentTest, FourParamInitialization) {
    Assignment assignment2{"Homework 1", std::chrono::sys_days{2025y/10/31}, false, 90.50};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::sys_days{2025y/10/31});
    ASSERT_EQ(assignment2.getCompleted(), false);
    ASSERT_EQ(assignment2.getGrade(), 90.50);
}

// initializations with description defined
TEST_F(AssignmentTest, TwoParamDescInitialization) {
    Assignment assignment2{"Homework 1", "Focus on lexical analysis."};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
}

TEST_F(AssignmentTest, ThreeParamDescInitialization) {
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", std::chrono::sys_days{2025y/10/31}};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::sys_days{2025y/10/31});
}

TEST_F(AssignmentTest, FourParamDescInitialization) {
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", std::chrono::sys_days{2025y/10/31},
                                        false};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::sys_days{2025y/10/31});
    ASSERT_EQ(assignment2.getCompleted(), false);
}

TEST_F(AssignmentTest, FiveParamDescInitialization) {
    Assignment assignment2{"Homework 1", "Focus on lexical analysis.", std::chrono::sys_days{2025y/10/31},
                                        false, 90.50};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::sys_days{2025y/10/31});
    ASSERT_EQ(assignment2.getCompleted(), false);
    ASSERT_EQ(assignment2.getGrade(), 90.50);
}

// ====================================
// FUNCTION SMOKE TESTS
// ====================================

TEST_F(AssignmentTest, PrintAssignmentInfo) {
    std::stringstream ss;
    CoutRedirect redirect(ss);

    assignment1.printAssignmentInfo();
    ASSERT_EQ(ss.str(), "===========================================================\nAssignment Title: Homework 3\nDescription: Focus on variables and strings.\n"
                        "Due Date: 2025-11-20\nCompleted? Yes\nGrade: 95.18\n===========================================================\n");
}

TEST_F(AssignmentTest, CompletedString) {
    Assignment assignment2{"Homework 1", std::chrono::sys_days{2025y/10/31}, false, 90.50};
    ASSERT_EQ(assignment1.completedString(assignment1.getCompleted()), "Yes");
    ASSERT_EQ(assignment2.completedString(assignment2.getCompleted()), "No");
}

// ====================================
// GETTER EDGE CASES
// ====================================

// empty parameters
TEST_F(AssignmentTest, TitleGetterEmpty) {
    Assignment assignment2{"", "Focus on lexical analysis.", std::chrono::sys_days{2025y/10/31},
                                        false, 90.50};
    // throw invalid argument error since title should not be empty
    ASSERT_THROW(assignment2.getTitle(), std::invalid_argument);
}

TEST_F(AssignmentTest, DescriptionGetterEmpty) {
    Assignment assignment2{"Homework 1", std::chrono::sys_days{2025y/10/31},
                                        false, 90.50};
    ASSERT_EQ(assignment2.getDescription(), "");
}

TEST_F(AssignmentTest, DueDateGetterEmpty) {
    Assignment assignment2{"Homework 1"};
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::time_point_cast<std::chrono::days>(now);    // set to today
    ASSERT_EQ(assignment2.getDueDate(), today);
}

TEST_F(AssignmentTest, CompletedGetterEmpty) {
    Assignment assignment2{"Homework 1", std::chrono::sys_days{2025y/2/30}};
    ASSERT_EQ(assignment2.getCompleted(), false);
}

TEST_F(AssignmentTest, GradeGetterEmpty) {
    Assignment assignment2{"Homework 1", std::chrono::sys_days{2025y/2/30}, false};
    ASSERT_EQ(assignment2.getGrade(), 0.0);
}

// invalid parameters
TEST_F(AssignmentTest, DueDateGetterInvalid) {
    // throw invalid argument since date does not exist
    ASSERT_THROW((Assignment assignment2{"Homework 1", std::chrono::sys_days{2025y/2/30}, false, 90.50}), std::invalid_argument;
}

TEST_F(AssignmentTest, GradeGetterInvalidLow) {
    // throw out of range error since number is not in range 0 to 100
    ASSERT_THROW((Assignment{"Homework 1", std::chrono::sys_days{2025y/2/30}, false, -20.24}), std::out_of_range);
}

TEST_F(AssignmentTest, GradeGetterInvalidHigh) {
    // throw out of range error since number is not in range 0 to 100
    ASSERT_THROW((Assignment{"Homework 1", std::chrono::sys_days{2025y/2/30}, false, 200.24}), std::out_of_range);
}

// ====================================
// SETTER EDGE CASES
// ====================================

TEST_F(AssignmentTest, TitleSetterInvalid) {
    // throw invalid argument since title is empty
    ASSERT_THROW(assignment1.setTitle(""), std::invalid_argument);
    ASSERT_EQ(assignment1.getTitle(), "Homework 3");
}

TEST_F(AssignmentTest, DueDateSetterInvalid) {
    // throw invalid argument since date does not exist
    ASSERT_THROW(assignment1.setDueDate(std::chrono::sys_days{2025y/2/30}), std::invalid_argument);
    ASSERT_EQ(assignment1.getDueDate(), std::chrono::sys_days{2025y/11/20});
}

TEST_F(AssignmentTest, GradeSetterInvalidLow) {
    // throw out of range since input is not in range 0 to 100
    ASSERT_THROW(assignment1.setGrade(-20.24), std::out_of_range);
    ASSERT_EQ(assignment1.getGrade(), 95.18);
}

TEST_F(AssignmentTest, GradeSetterInvalidHigh) {
    // throw out of range since input is not in range 0 to 100
    ASSERT_THROW(assignment1.setGrade(200.24), std::out_of_range);
    ASSERT_EQ(assignment1.getGrade(), 95.18);
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
    Assignment assignment2{"Homework 1", std::chrono::sys_days{2025y/2/30}};
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    // throw invalid argument since date does not exist 
    ASSERT_THROW(assignment2.getDueDate(), std::invalid_argument);
}

TEST_F(AssignmentTest, FourParamInitializationInvalidLow) {
    // throw out of range since input is not in range 0 to 100
    ASSERT_THROW((Assignment assignment2{"Homework 1", std::chrono::sys_days{2025y/10/31}, false, -20.24}), std::out_of_range);
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::sys_days{2025y/10/31});
    ASSERT_EQ(assignment2.getCompleted(), false);
}

TEST_F(AssignmentTest, FourParamInitializationInvalidHigh) {
    // throw out of range since input is not in range 0 to 100
    ASSERT_THROW((Assignment assignment2{"Homework 1", std::chrono::sys_days{2025y/10/31}, false, 200.24}), std::out_of_range);
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::sys_days{2025y/10/31});
    ASSERT_EQ(assignment2.getCompleted(), false);
}

// invalid initializations with description defined
TEST_F(AssignmentTest, ThreeParamDescInitializationInvalid) {
    // throw invalid argument since date does not exist
    ASSERT_THROW((Assignment assignment2{"Homework 1", "Focus on lexical analysis.", std::chrono::sys_days{2025y/2/30}}), std::invalid_argument);
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
}

TEST_F(AssignmentTest, FiveParamDescInitializationInvalidLow) {
    // throw out of range since input is not in range 0 to 100
    ASSERT_THROW((Assignment assignment2{"Homework 1", "Focus on lexical analysis.", std::chrono::sys_days{2025y/10/31},
                                        false, -20.24}), std::out_of_range);
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::sys_days{2025y/10/31});
    ASSERT_EQ(assignment2.getCompleted(), false);
}

TEST_F(AssignmentTest, FiveParamDescInitializationInvalidHigh) {
    // throw out of range since input is not in range 0 to 100
    ASSERT_THROW((Assignment assignment2{"Homework 1", "Focus on lexical analysis.", std::chrono::sys_days{2025y/10/31},
                                        false, 200.24}), std::out_of_range);
    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::sys_days{2025y/10/31});
    ASSERT_EQ(assignment2.getCompleted(), false);
}

// ====================================
// FUNCTION EDGE CASES
// ====================================

// ====================================
// CLASS USE CASES
// ====================================

int main(int argc, char **argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}