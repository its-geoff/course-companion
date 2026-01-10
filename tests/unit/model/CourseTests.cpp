#include <gtest/gtest.h>
#include <sstream>      // diverts output from terminal to separate stream
#include <regex>        // regular expression matching for UUIDs
#include "model/Course.hpp"
#include "utils/utils.hpp"

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

TEST_F(CourseTest, IdGetter) {
    // ensure ID is not empty
    ASSERT_FALSE(utils::isOnlyWhitespace(course1.getId()));
}

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

TEST_F(CourseTest, AssignmentListGetter) {
    Assignment assignment1{"Homework 3", "Focus on variables and strings.", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};
    Assignment assignment2{"Homework 1", "", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};
    course1.addAssignment(assignment1);
    course1.addAssignment(assignment2);
    std::string id1 = assignment1.getId();
    std::string id2 = assignment2.getId();

    ASSERT_EQ(course1.findAssignment(id1).getTitle(), "Homework 3");
    ASSERT_EQ(course1.findAssignment(id2).getDueDate(), std::chrono::year_month_day{2025y/10/31});
}

TEST_F(CourseTest, AssignmentListGetterCheckSize) {
    Assignment assignment1{"Homework 3", "Focus on variables and strings.", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};
    Assignment assignment2{"Homework 1", "", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};
    course1.addAssignment(assignment1);
    course1.addAssignment(assignment2);
    auto list = course1.getAssignmentList();
    ASSERT_EQ(list.size(), 2);
}

TEST_F(CourseTest, GradeWeightsGetter) {
    auto weights = course1.getGradeWeights();

    // check that these key-value pairs are in the map
    ASSERT_TRUE(weights.find("Homework") != weights.end());
    ASSERT_TRUE(weights.find("Final Exam") != weights.end());
}

TEST_F(CourseTest, NumCreditsGetter) {
    ASSERT_EQ(course1.getNumCredits(), 3);
}

TEST_F(CourseTest, GradePctGetter) {
    ASSERT_FLOAT_EQ(course1.getGradePct(), 0.0);
}

TEST_F(CourseTest, LetterGradeGetter) {
    course1.setGradePct(86.0f);
    course1.setLetterGrade();
    ASSERT_EQ(course1.getLetterGrade(), "B");
}

TEST_F(CourseTest, GpaValGetter) {
    course1.setGradePct(88.3f);
    course1.setLetterGrade();
    course1.setGpaVal();
    ASSERT_FLOAT_EQ(course1.getGpaVal(), 3.3f);
}

TEST_F(CourseTest, ActiveGetter) {
    ASSERT_FALSE(course1.getActive());
}

TEST_F(CourseTest, GradeScaleGetter) {
    std::map<float, std::string> gradeScale2 = {
        {97.0, "A+"},
        {93.0, "A"},
        {90.0, "A-"},
        {87.0, "B+"},
        {83.0, "B"},
        {80.0, "B-"},
        {77.0, "C+"},
        {73.0, "C"},
        {70.0, "C-"},
        {67.0, "D+"},
        {63.0, "D"},
        {60.0, "D-"},
        {0.0, "F"}
    };

    ASSERT_EQ(course1.getGradeScale(), gradeScale2);
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

TEST_F(CourseTest, GradeWeightsSetter) {
    std::unordered_map<std::string, float> gradeWeights2 = {
        {"Homework", 0.2},
        {"Midterm", 0.4},
        {"Final Exam", 0.4}
    };

    course1.setGradeWeights(gradeWeights2);
    auto weights = course1.getGradeWeights();
    // check that the values have been updated
    ASSERT_FLOAT_EQ(weights.find("Homework")->second, 0.2);
    ASSERT_FLOAT_EQ(weights.find("Midterm")->second, 0.4);
}

TEST_F(CourseTest, GradePctSetter) {
    course1.setGradePct(89.17f);
    ASSERT_FLOAT_EQ(course1.getGradePct(), 89.17f);
}

TEST_F(CourseTest, LetterGradeSetter) {
    course1.setGradePct(90.18f);
    course1.setLetterGrade();   // calculateLetterGrade used in setLetterGrade
    ASSERT_EQ(course1.getLetterGrade(), "A-");
}

TEST_F(CourseTest, GpaValSetter) {
    course1.setGradePct(75.28f);
    course1.setLetterGrade();
    course1.setGpaVal(); // calculateGpaVal used in setGpaVal
    ASSERT_EQ(course1.getGpaVal(), 2.0f);
}

TEST_F(CourseTest, ActiveSetter) {
    course1.setActive(true);
    ASSERT_TRUE(course1.getActive());
}

TEST_F(CourseTest, GradeScaleSetter) {
    Course course2{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, 1};
    std::map<float, std::string> gradeScale2 = {
        {70.0, "P"},
        {0.0, "NP"}
    };

    course2.setGradeScale(gradeScale2);
    ASSERT_EQ(course2.getGradeScale(), gradeScale2);
}

// ====================================
// INITIALIZATION SMOKE TESTS
// ====================================

TEST_F(CourseTest, ThreeParamInitialization) {
    Course course2{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    ASSERT_FALSE(utils::isOnlyWhitespace(course2.getId()));
    ASSERT_EQ(course2.getTitle(), "ENGR 195A");
    ASSERT_EQ(course2.getDescription(), "");
    ASSERT_EQ(course2.getStartDate(), std::chrono::year_month_day{2025y/8/14});
    ASSERT_EQ(course2.getEndDate(), std::chrono::year_month_day{2025y/12/18});
}

TEST_F(CourseTest, FourParamInitialization) {
    Course course2{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, 1};
    ASSERT_FALSE(utils::isOnlyWhitespace(course2.getId()));
    ASSERT_EQ(course2.getTitle(), "ENGR 195A");
    ASSERT_EQ(course2.getDescription(), "");
    ASSERT_EQ(course2.getStartDate(), std::chrono::year_month_day{2025y/8/14});
    ASSERT_EQ(course2.getEndDate(), std::chrono::year_month_day{2025y/12/18});
    ASSERT_EQ(course2.getNumCredits(), 1);
}

TEST_F(CourseTest, FiveParamInitialization) {
    Course course2{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, 1, false};
    ASSERT_FALSE(utils::isOnlyWhitespace(course2.getId()));
    ASSERT_EQ(course2.getTitle(), "ENGR 195A");
    ASSERT_EQ(course2.getDescription(), "");
    ASSERT_EQ(course2.getStartDate(), std::chrono::year_month_day{2025y/8/14});
    ASSERT_EQ(course2.getEndDate(), std::chrono::year_month_day{2025y/12/18});
    ASSERT_EQ(course2.getNumCredits(), 1);
    ASSERT_FALSE(course2.getActive());
}

TEST_F(CourseTest, FourParamDescInitialization) {
    Course course2{"ENGR 195A", "Global and Social Issues in Engineering", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    ASSERT_FALSE(utils::isOnlyWhitespace(course2.getId()));
    ASSERT_EQ(course2.getTitle(), "ENGR 195A");
    ASSERT_EQ(course2.getDescription(), "Global and Social Issues in Engineering");
    ASSERT_EQ(course2.getStartDate(), std::chrono::year_month_day{2025y/8/14});
    ASSERT_EQ(course2.getEndDate(), std::chrono::year_month_day{2025y/12/18});
}

TEST_F(CourseTest, FiveParamDescInitialization) {
    Course course2{"ENGR 195A", "Global and Social Issues in Engineering", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, 1};
    ASSERT_FALSE(utils::isOnlyWhitespace(course2.getId()));
    ASSERT_EQ(course2.getTitle(), "ENGR 195A");
    ASSERT_EQ(course2.getDescription(), "Global and Social Issues in Engineering");
    ASSERT_EQ(course2.getStartDate(), std::chrono::year_month_day{2025y/8/14});
    ASSERT_EQ(course2.getEndDate(), std::chrono::year_month_day{2025y/12/18});
    ASSERT_EQ(course2.getNumCredits(), 1);
}

TEST_F(CourseTest, SixParamDescInitialization) {
    Course course2{"ENGR 195A", "Global and Social Issues in Engineering", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, 1, false};
    ASSERT_FALSE(utils::isOnlyWhitespace(course2.getId()));
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

TEST_F(CourseTest, PrintCourseInfo) {
    std::stringstream ss;

    course1.printCourseInfo(ss);
    std::string output = ss.str();

    // find and replace the UUID part
    std::regex uuidRegex("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    output = std::regex_replace(output, uuidRegex, "<UUID>");

    ASSERT_EQ(output, "ID: <UUID>\nCourse: CMPE 142\nDescription: Operating Systems\n"
                        "Duration: 2025-08-12 - 2025-12-05\nNumber of Credits: 3\nGrade Percentage: 0.00%\nLetter Grade: \n"
                        "GPA Value: 0.0\nCurrent? No\n");
}

TEST_F(CourseTest, AddAssignment) {
    Assignment assignment1{"Homework 3", "Focus on variables and strings.", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};

    course1.addAssignment(assignment1);
    ASSERT_EQ(course1.getAssignmentList().size(), 1);
}

TEST_F(CourseTest, RemoveAssignment) {
    Assignment assignment1{"Homework 3", "Focus on variables and strings.", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};
    Assignment assignment2{"Homework 1", "", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};

    course1.addAssignment(assignment1);
    course1.addAssignment(assignment2);
    std::string id = assignment1.getId();
    course1.removeAssignment(id);

    // check size and success of removal
    ASSERT_EQ(course1.getAssignmentList().size(), 1);
    ASSERT_THROW(course1.getAssignmentList().at(id), std::out_of_range);
}

TEST_F(CourseTest, FindAssignmentConst) {
    Assignment assignment1{"Homework 3", "Focus on variables and strings.", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};
    Assignment assignment2{"Homework 1", "", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};

    course1.addAssignment(assignment1);
    course1.addAssignment(assignment2);
    std::string id = assignment1.getId();

    // cast to const Course to use const version of function
    ASSERT_EQ(static_cast<const Course&>(course1).findAssignment(id), assignment1);
}

TEST_F(CourseTest, FindAssignmentNonConst) {
    Assignment assignment1{"Homework 3", "Focus on variables and strings.", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};
    Assignment assignment2{"Homework 1", "", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};
    Assignment assignment3 = assignment1;    // make assignment3 the same as assignment1

    course1.addAssignment(assignment1);
    course1.addAssignment(assignment2);
    std::string id = assignment1.getId();
    course1.findAssignment(id).setCompleted(false);
    assignment3.setCompleted(false);    // copy change to assignment1

    ASSERT_EQ(course1.findAssignment(id), assignment3);
}

TEST_F(CourseTest, OverloadedEquals) {
    Course course2{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    Course course3{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course4 = course1;

    ASSERT_FALSE(course1 == course2);
    ASSERT_FALSE(course1 == course3);
    ASSERT_TRUE(course1 == course4);
}

// ====================================
// GETTER EDGE CASES
// ====================================

// empty parameters
TEST_F(CourseTest, DescriptionGetterEmpty) {
    Course course2{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    ASSERT_EQ(course2.getDescription(), "");
}

TEST_F(CourseTest, NumCreditsGetterEmpty) {
    Course course2{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    ASSERT_EQ(course2.getNumCredits(), 3);
}

TEST_F(CourseTest, ActiveGetterEmpty) {
    Course course2{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    ASSERT_TRUE(course2.getActive());
}

// ====================================
// SETTER EDGE CASES
// ====================================

TEST_F(CourseTest, TitleSetterInvalid) {
    // throw invalid argument since title is empty
    ASSERT_THROW(course1.setTitle(""), std::invalid_argument);
    ASSERT_EQ(course1.getTitle(), "CMPE 142");
}

TEST_F(CourseTest, TitleSetterWhitespaceInvalid) {
    // throw invalid argument since title is whitespace
    ASSERT_THROW(course1.setTitle(" "), std::invalid_argument);
    ASSERT_EQ(course1.getTitle(), "CMPE 142");
}

TEST_F(CourseTest, DescriptionSetterEmpty) {
    course1.setDescription("");
    // check if description is empty
    ASSERT_TRUE(course1.getDescription().empty());
}

TEST_F(CourseTest, DescriptionSetterWhitespace) {
    course1.setDescription("  ");
    // check if description is empty
    ASSERT_TRUE(course1.getDescription().empty());
}

TEST_F(CourseTest, StartDateSetterInvalid) {
    // throw invalid argument since date does not exist
    ASSERT_THROW(course1.setStartDate(std::chrono::year_month_day{2025y/2/30}), std::invalid_argument);
    ASSERT_EQ(course1.getStartDate(), std::chrono::year_month_day{2025y/8/12});
}

TEST_F(CourseTest, EndDateSetterInvalid) {
    // throw invalid argument since date does not exist
    ASSERT_THROW(course1.setEndDate(std::chrono::year_month_day{2025y/2/30}), std::invalid_argument);
    ASSERT_EQ(course1.getEndDate(), std::chrono::year_month_day{2025y/12/5});
}

TEST_F(CourseTest, GradeWeightsSetterInvalid) {
    Course course2{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, 1};
    std::unordered_map<std::string, float> gradeWeights2 = {
        {"Homework", 0.4},
        {"Midterm", 0.4},
        {"Final Exam", 0.4}
    };

    // throw invalid argument since grade percentages > 100%
    ASSERT_THROW(course2.setGradeWeights(gradeWeights2), std::invalid_argument);
    auto weights = course2.getGradeWeights();
    // check that the values have not been updated
    ASSERT_FLOAT_EQ(weights.find("Homework")->second, 0.25);
    ASSERT_FLOAT_EQ(weights.find("Midterm")->second, 0.35);
}

TEST_F(CourseTest, NumCreditsSetterInvalid) {
    // throw out of range since input is not greater than or equal to 0
    ASSERT_THROW(course1.setNumCredits(-3), std::out_of_range);
    ASSERT_EQ(course1.getNumCredits(), 3);
}

TEST_F(CourseTest, NumCreditsSetterZero) {
    course1.setNumCredits(0);
    ASSERT_EQ(course1.getNumCredits(), 0);
}

TEST_F(CourseTest, GradePctSetterInvalidLow) {
    // throw out of range since input is not in range 0 to 100
    ASSERT_THROW(course1.setGradePct(-20.24f), std::out_of_range);
    ASSERT_FLOAT_EQ(course1.getGradePct(), 0.0f);
}

TEST_F(CourseTest, GradePctSetterInvalidHigh) {
    // throw out of range since input is not in range 0 to 100
    ASSERT_THROW(course1.setGradePct(200.24f), std::out_of_range);
    ASSERT_FLOAT_EQ(course1.getGradePct(), 0.0f);
}

TEST_F(CourseTest, GradePctSetterBoundaryLow) {
    course1.setGradePct(0.0f);
    ASSERT_FLOAT_EQ(course1.getGradePct(), 0.0f);
}

TEST_F(CourseTest, GradePctSetterBoundaryHigh) {
    course1.setGradePct(100.0f);
    ASSERT_FLOAT_EQ(course1.getGradePct(), 100.0f);
}

TEST_F(CourseTest, LetterGradeSetterRangeHigh) {
    course1.setGradePct(92.99f);
    course1.setLetterGrade();
    ASSERT_EQ(course1.getLetterGrade(), "A-");
}

TEST_F(CourseTest, LetterGradeSetterLowF) {
    course1.setGradePct(35.0f);
    course1.setLetterGrade();
    ASSERT_EQ(course1.getLetterGrade(), "F");
}

TEST_F(CourseTest, LetterGradeSetterBoundaryHigh) {
    course1.setGradePct(100.0f);
    course1.setLetterGrade();
    ASSERT_EQ(course1.getLetterGrade(), "A+");
}

TEST_F(CourseTest, LetterGradeSetterBoundaryLow) {
    course1.setGradePct(0.0f);
    course1.setLetterGrade();
    ASSERT_EQ(course1.getLetterGrade(), "F");
}

TEST_F(CourseTest, GpaValSetterNoGrade) {
    // expected path is gradePct -> letterGrade -> gpaVal
    // going straight from gradePct to gpaVal should call setLetterGrade
    course1.setGradePct(88.4f);
    course1.setGpaVal();
    ASSERT_FLOAT_EQ(course1.getGpaVal(), 3.3);
}

TEST_F(CourseTest, GpaValSetterLowF) {
    course1.setGradePct(15.5f);
    course1.setLetterGrade();
    course1.setGpaVal();
    ASSERT_EQ(course1.getGpaVal(), 0.0f);
}

TEST_F(CourseTest, GradeScaleSetterEmpty) {
    Course course2{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, 1};
    std::map<float, std::string> gradeScale2 = {};

    ASSERT_THROW(course2.setGradeScale(gradeScale2), std::runtime_error);
}

TEST_F(CourseTest, GradeScaleSetterMissingZero) {
    Course course2{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, 1};
    std::map<float, std::string> gradeScale2 = {
        {80.0, "A"},
        {60.0, "C"}
    };

    ASSERT_THROW(course2.setGradeScale(gradeScale2), std::runtime_error);
}

TEST_F(CourseTest, GradeScaleSetterInvalidHigh) {
    Course course2{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, 1};
    std::map<float, std::string> gradeScale2 = {
        {110.0, "A++"},
        {60.0, "C"},
        {0.0, "F"}
    };

    ASSERT_THROW(course2.setGradeScale(gradeScale2), std::runtime_error);
}

TEST_F(CourseTest, GradeScaleSetterUpperBound) {
    Course course2{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, 1};
    std::map<float, std::string> gradeScale2 = {
        {100.0, "A++"},
        {90.0, "A-"},
        {0.0, "F"}
    };

    ASSERT_THROW(course2.setGradeScale(gradeScale2), std::runtime_error);
}

// ====================================
// INITIALIZATION EDGE CASES
// ====================================

TEST_F(CourseTest, ThreeParamInitializationNoTitle) {
    // throw invalid argument since title is empty
    ASSERT_THROW((Course{"", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}}), std::invalid_argument);
}

TEST_F(CourseTest, ThreeParamInitializationInvalidStartDate) {
    // throw invalid argument since start date does not exist
    ASSERT_THROW((Course{"ENGR 195A", "", std::chrono::year_month_day{2025y/2/31}, std::chrono::year_month_day{2025y/12/18}}), std::invalid_argument);
}

TEST_F(CourseTest, ThreeParamInitializationInvalidEndDate) {
    // throw invalid argument since end date does not exist
    ASSERT_THROW((Course{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2026y/2/31}}), std::invalid_argument);
}

TEST_F(CourseTest, ThreeParamInitializationDefaultDates) {
    Course course2{"ENGR 195A", "", {}, {}};

    std::chrono::year_month_day todayDate = utils::getTodayDate();
    std::chrono::year_month_day defaultEnd = todayDate + std::chrono::months{4};
    ASSERT_EQ(course2.getTitle(), "ENGR 195A");
    ASSERT_EQ(course2.getDescription(), "");
    ASSERT_EQ(course2.getStartDate(), todayDate);
    ASSERT_EQ(course2.getEndDate(), defaultEnd);
    ASSERT_EQ(course2.getNumCredits(), 3);
    ASSERT_EQ(course2.getActive(), true);
}

TEST_F(CourseTest, FourParamInitializationInvalidNumCredits) {
    // throw out of range since input is not greater than or equal to 0
    ASSERT_THROW((Course{"ENGR 195A", "", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, -4}), std::out_of_range);
}

TEST_F(CourseTest, FourParamDescInitializationNoTitle) {
    // throw invalid argument since title is empty
    ASSERT_THROW((Course{"", "Global and Social Issues in Engineering", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}}), std::invalid_argument);
}

TEST_F(CourseTest, FourParamDescInitializationInvalidStartDate) {
    // throw invalid argument since start date does not exist
    ASSERT_THROW((Course{"ENGR 195A", "Global and Social Issues in Engineering", std::chrono::year_month_day{2025y/2/31}, std::chrono::year_month_day{2025y/12/18}}), std::invalid_argument);
}

TEST_F(CourseTest, FourParamDescInitializationInvalidEndDate) {
    // throw invalid argument since end date does not exist
    ASSERT_THROW((Course{"ENGR 195A", "Global and Social Issues in Engineering", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2026y/2/31}}), std::invalid_argument);
}

TEST_F(CourseTest, FourParamDescInitializationDefaultDates) {
    Course course2{"ENGR 195A", "Global and Social Issues in Engineering", {}, {}};

    std::chrono::year_month_day todayDate = utils::getTodayDate();
    std::chrono::year_month_day defaultEnd = todayDate + std::chrono::months{4};
    ASSERT_EQ(course2.getTitle(), "ENGR 195A");
    ASSERT_EQ(course2.getDescription(), "Global and Social Issues in Engineering");
    ASSERT_EQ(course2.getStartDate(), todayDate);
    ASSERT_EQ(course2.getEndDate(), defaultEnd);
    ASSERT_EQ(course2.getNumCredits(), 3);
    ASSERT_EQ(course2.getActive(), true);
}

TEST_F(CourseTest, FiveParamDescInitializationInvalidNumCredits) {
    // throw out of range since input is not greater than or equal to 0
    ASSERT_THROW((Course{"ENGR 195A", "Global and Social Issues in Engineering", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}, -4}), std::out_of_range);
}

// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST_F(CourseTest, PrintCourseInfoPartial) {
    std::stringstream ss;
    Course course2{"CMPE 142", "", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}};
    course2.printCourseInfo(ss);
    std::string output = ss.str();

    // find and replace the UUID part
    std::regex uuidRegex("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    output = std::regex_replace(output, uuidRegex, "<UUID>");

    ASSERT_EQ(output, "ID: <UUID>\nCourse: CMPE 142\n"
                        "Duration: 2025-08-12 - 2025-12-05\nNumber of Credits: 3\nGrade Percentage: 0.00%\nLetter Grade: \n"
                        "GPA Value: 0.0\nCurrent? Yes\n");
}

TEST_F(CourseTest, PrintCourseInfoDescPartial) {
    std::stringstream ss;
    Course course2{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}};
    course2.printCourseInfo(ss);
    std::string output = ss.str();

    // find and replace the UUID part
    std::regex uuidRegex("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    output = std::regex_replace(output, uuidRegex, "<UUID>");

    ASSERT_EQ(output, "ID: <UUID>\nCourse: CMPE 142\nDescription: Operating Systems\n"
                        "Duration: 2025-08-12 - 2025-12-05\nNumber of Credits: 3\nGrade Percentage: 0.00%\nLetter Grade: \n"
                        "GPA Value: 0.0\nCurrent? Yes\n");
}

TEST_F(CourseTest, AddAssignmentAlreadyExists) {
    Assignment assignment1{"Homework 3", "Focus on variables and strings.", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};

    course1.addAssignment(assignment1);

    // throw logic error since Assignment already exists in list
    ASSERT_THROW(course1.addAssignment(assignment1), std::logic_error);
}

TEST_F(CourseTest, RemoveAssignmentNotFound) {
    Assignment assignment1{"Homework 3", "Focus on variables and strings.", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};
    Assignment assignment2{"Homework 1", "", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};
    std::string id = assignment2.getId();

    course1.addAssignment(assignment1);

    // throw out of range since ID was not found
    ASSERT_THROW(course1.removeAssignment(id), std::out_of_range);
}

TEST_F(CourseTest, FindAssignmentConstNotFound) {
    Assignment assignment1{"Homework 3", "Focus on variables and strings.", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};
    Assignment assignment2{"Homework 1", "", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};

    course1.addAssignment(assignment1);
    std::string id = assignment2.getId();

    // cast to const Course to use const version of function
    // throw out of range since ID was not found
    ASSERT_THROW(static_cast<const Course&>(course1).findAssignment(id), std::out_of_range);
}

TEST_F(CourseTest, FindAssignmentNonConstNotFound) {
    Assignment assignment1{"Homework 3", "Focus on variables and strings.", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};
    Assignment assignment2{"Homework 1", "", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};

    course1.addAssignment(assignment1);
    std::string id = assignment2.getId();

    // throw out of range since ID was not found
    ASSERT_THROW(course1.findAssignment(id), std::out_of_range);
}

TEST_F(CourseTest, OverloadedEqualsSameTitleDifferentParams) {
    Course course2{"CMPE 142", "Global and Social Issues in Engineering", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course3{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/9/2}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course4{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2024y/12/5}, 3, false};
    Course course5{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 1, false};
    Course course6{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, true};

    ASSERT_FALSE(course1 == course2);
    ASSERT_FALSE(course1 == course3);
    ASSERT_FALSE(course1 == course4);
    ASSERT_FALSE(course1 == course5);
    ASSERT_FALSE(course1 == course6);
}

TEST_F(CourseTest, OverloadedEqualsSameParamsDifferentId) {
    Course course2{"CMPE 142", "Global and Social Issues in Engineering", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course3{"CMPE 142", "Global and Social Issues in Engineering", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};

    ASSERT_FALSE(course2 == course3);
}