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

TEST_F(AssignmentTest, ValidateGradeLow) {
    Assignment assignment2{"Homework 1", std::chrono::sys_days{2025y/10/31}, false, -20.24};
    ASSERT_THROW(assignment2.validateGrade(assignment2.getGrade()), std::out_of_range);
}

TEST_F(AssignmentTest, ValidateGradeHigh) {
    Assignment assignment2{"Homework 1", std::chrono::sys_days{2025y/10/31}, false, 200.24};
    ASSERT_THROW(assignment2.validateGrade(assignment2.getGrade()), std::out_of_range);
}

TEST_F(AssignmentTest, ValidateTitle) {
    Assignment assignment2{"", std::chrono::sys_days{2025y/10/31}, false, 90.50};
    ASSERT_THROW(assignment2.validateTitle(assignment2.getTitle()), std::invalid_argument);
}

TEST_F(AssignmentTest, CompletedString) {
    Assignment assignment2{"", std::chrono::sys_days{2025y/10/31}, false, 90.50};
    ASSERT_EQ(assignment1.completedString(assignment1.getCompleted()), "Yes");
    ASSERT_EQ(assignment2.completedString(assignment2.getCompleted()), "No");
}

// ====================================
// GETTER EDGE CASES
// ====================================



// ====================================
// SETTER EDGE CASES
// ====================================

// ====================================
// INITIALIZATION EDGE CASES
// ====================================

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