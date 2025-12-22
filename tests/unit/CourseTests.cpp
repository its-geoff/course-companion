#include <gtest/gtest.h>
#include <sstream>
#include "models/Course.hpp"

using namespace std::chrono_literals;

// test fixture for class Course
class CourseTest : public testing::Test {
    protected:
        Course course1{
            "CMPE 142", 
            "Operating Systems",
            std::chrono::year_month_day{2025y/8/12},
            std::chrono::year_month_day{2025y/12/5},
            3,
            false
        };
};

// ====================================
// GETTER SMOKE TESTS
// ====================================

TEST_F(CourseTest, TitleGetter) {
    ASSERT_EQ(course1.getTitle(), "CMPE 142");
}

TEST_F(CourseTest, DescriptionGetter) {
    ASSERT_EQ(course1.getDescription(), "Operating Systems");
}

TEST_F(CourseTest, StartDateGetter) {
    ASSERT_EQ(course1.getStartDate(), std::chrono::year_month_day{2025y/8/12});
}

TEST_F(CourseTest, EndDateGetter) {
    ASSERT_EQ(course1.getEndDate(), std::chrono::year_month_day{2025y/12/5});
}

// insert assignmentList getter

TEST_F(CourseTest, NumCreditsGetter) {
    ASSERT_EQ(course1.getNumCredits(), 3);
}

// insert gradingScale getter

TEST_F(CourseTest, GradePctGetter) {
    ASSERT_FLOAT_EQ(course1.getGradePct(), 0.0);
}

// insert letterGrade getter

TEST_F(CourseTest, ActiveGetter) {
    ASSERT_FALSE(course1.getActive());
}

// ====================================
// SETTER SMOKE TESTS
// ====================================

TEST_F(CourseTest, TitleSetter) {
    course1.setTitle("HSPM 149");
    ASSERT_EQ(course1.getTitle(), "HSPM 149");
}

TEST_F(CourseTest, DescriptionSetter) {
    course1.setDescription("Beer Appreciation");
    ASSERT_EQ(course1.getDescription(), "Beer Appreciation");
}

TEST_F(CourseTest, StartDateSetter) {
    course1.setStartDate(std::chrono::year_month_day{2025y/9/25});
    ASSERT_EQ(course1.getStartDate(), std::chrono::year_month_day{2025y/9/25});
}

TEST_F(CourseTest, EndDateSetter) {
    course1.setEndDate(std::chrono::year_month_day{2025y/11/7});
    ASSERT_EQ(course1.getEndDate(), std::chrono::year_month_day{2025y/11/7});
}

TEST_F(CourseTest, NumCreditsSetter) {
    course1.setNumCredits(2);
    ASSERT_EQ(course1.getNumCredits(), 2);
}

TEST_F(CourseTest, GradePctSetter) {
    course1.setGradePct(89.17f);
    ASSERT_FLOAT_EQ(course1.getGradePct(), 89.17f);
}

// insert letterGrade setter
// insert gradingScale setter

TEST_F(CourseTest, ActiveSetter) {
    course1.setActive(true);
    ASSERT_TRUE(course1.getActive());
}

// ====================================
// INITIALIZATION SMOKE TESTS
// ====================================

// initializations without description defined
TEST_F(CourseTest, ThreeParamInitialization) {
    Course course2{"ENGR 195A", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    ASSERT_EQ(course2.getTitle(), "ENGR 195A");
    ASSERT_EQ(course2.getStartDate(), std::chrono::year_month_day{2025y/8/14});
    ASSERT_EQ(course2.getEndDate(), std::chrono::year_month_day{2025y/12/18});
}

TEST_F(CourseTest, FourParamInitialization) {
    Course course2{"ENGR 195A", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, 1};
    ASSERT_EQ(course2.getTitle(), "ENGR 195A");
    ASSERT_EQ(course2.getStartDate(), std::chrono::year_month_day{2025y/8/14});
    ASSERT_EQ(course2.getEndDate(), std::chrono::year_month_day{2025y/12/18});
    ASSERT_EQ(course2.getNumCredits(), 1);
}

TEST_F(CourseTest, FiveParamInitialization) {
    Course course2{"ENGR 195A", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, 1, false};
    ASSERT_EQ(course2.getTitle(), "ENGR 195A");
    ASSERT_EQ(course2.getStartDate(), std::chrono::year_month_day{2025y/8/14});
    ASSERT_EQ(course2.getEndDate(), std::chrono::year_month_day{2025y/12/18});
    ASSERT_EQ(course2.getNumCredits(), 1);
    ASSERT_FALSE(course2.getActive());
}

// initializations with description defined
TEST_F(CourseTest, FourParamDescInitialization) {
    Course course2{"ENGR 195A", "Global and Social Issues in Engineering", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    ASSERT_EQ(course2.getTitle(), "ENGR 195A");
    ASSERT_EQ(course2.getDescription(), "Global and Social Issues in Engineering");
    ASSERT_EQ(course2.getStartDate(), std::chrono::year_month_day{2025y/8/14});
    ASSERT_EQ(course2.getEndDate(), std::chrono::year_month_day{2025y/12/18});
}

TEST_F(CourseTest, FiveParamDescInitialization) {
    Course course2{"ENGR 195A", "Global and Social Issues in Engineering", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, 1};
    ASSERT_EQ(course2.getTitle(), "ENGR 195A");
    ASSERT_EQ(course2.getDescription(), "Global and Social Issues in Engineering");
    ASSERT_EQ(course2.getStartDate(), std::chrono::year_month_day{2025y/8/14});
    ASSERT_EQ(course2.getEndDate(), std::chrono::year_month_day{2025y/12/18});
    ASSERT_EQ(course2.getNumCredits(), 1);
}

TEST_F(CourseTest, SixParamDescInitialization) {
    Course course2{"ENGR 195A", "Global and Social Issues in Engineering", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, 1, false};
    ASSERT_EQ(course2.getTitle(), "ENGR 195A");
    ASSERT_EQ(course2.getDescription(), "Global and Social Issues in Engineering");
    ASSERT_EQ(course2.getStartDate(), std::chrono::year_month_day{2025y/8/14});
    ASSERT_EQ(course2.getEndDate(), std::chrono::year_month_day{2025y/12/18});
    ASSERT_EQ(course2.getNumCredits(), 1);
    ASSERT_FALSE(course2.getActive());
}

// ====================================
// FUNCTION SMOKE TESTS
// ====================================


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

