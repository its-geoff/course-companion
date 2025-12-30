#include <gtest/gtest.h>
#include <sstream>      // diverts output from terminal to separate stream
#include <regex>        // regular expression matching for UUIDs
#include "models/Term.hpp"
#include "app/utils.hpp"

using namespace std::chrono_literals;

// test fixture for class Term
class TermTest : public testing::Test {
    protected:
        Term term1{
            "Fall 2025", 
            std::chrono::year_month_day{2025y/8/12},
            std::chrono::year_month_day{2025y/12/5},
            false
        };
};

// ====================================
// GETTER SMOKE TESTS
// ====================================

TEST_F(TermTest, IdGetter) {
    // ensure ID is not empty
    ASSERT_FALSE(utils::isOnlyWhitespace(term1.getId()));
}

TEST_F(TermTest, TitleGetter) {
    ASSERT_EQ(term1.getTitle(), "Fall 2025");
}

TEST_F(TermTest, StartDateGetter) {
    ASSERT_EQ(term1.getStartDate(), std::chrono::year_month_day{2025y/8/12});
}

TEST_F(TermTest, EndDateGetter) {
    ASSERT_EQ(term1.getEndDate(), std::chrono::year_month_day{2025y/12/5});
}

TEST_F(TermTest, CourseListGetter) {
    Course course1{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course2{"ENGR 195A", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    term1.addCourse(course1);
    term1.addCourse(course2);
    std::string id1 = course1.getId();
    std::string id2 = course2.getId();

    ASSERT_EQ(term1.findCourse(id1).getDescription(), "Operating Systems");
    ASSERT_EQ(term1.findCourse(id2).getEndDate(), std::chrono::year_month_day{2025y/12/18});
}

TEST_F(TermTest, CourseListGetterCheckSize) {
    Course course1{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course2{"ENGR 195A", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    term1.addCourse(course1);
    term1.addCourse(course2);
    auto list = term1.getCourseList();
    ASSERT_EQ(list.size(), 2);
}

TEST_F(TermTest, TotalCreditsGetter) {
    Course course1{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course2{"ENGR 195A", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    term1.addCourse(course1);
    term1.addCourse(course2);

    ASSERT_EQ(term1.getTotalCredits(), 6);
}

TEST_F(TermTest, OvrGpaGetter) {
    Course course1{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course2{"ENGR 195A", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    term1.addCourse(course1);
    term1.addCourse(course2);

    ASSERT_EQ(term1.getOvrGpa(), 0);
}

TEST_F(TermTest, ActiveGetter) {
    ASSERT_FALSE(term1.getActive());
}

// ====================================
// SETTER SMOKE TESTS
// ====================================

TEST_F(TermTest, TitleSetter) {
    term1.setTitle("Spring 2026");
    ASSERT_EQ(term1.getTitle(), "Spring 2026");
}

TEST_F(TermTest, StartDateSetter) {
    term1.setStartDate(std::chrono::year_month_day{2026y/1/20});
    ASSERT_EQ(term1.getStartDate(), std::chrono::year_month_day{2026y / 1 / 20});
}

TEST_F(TermTest, EndDateSetter) {
    term1.setEndDate(std::chrono::year_month_day{2026y/5/23});
    ASSERT_EQ(term1.getEndDate(), std::chrono::year_month_day{2026y/5/23});
}

TEST_F(TermTest, ActiveSetter) {
    term1.setActive(false);
    ASSERT_FALSE(term1.getActive());
}

// ====================================
// INITIALIZATION SMOKE TESTS
// ====================================

TEST_F(TermTest, ThreeParamInitialization) {
    Term term2{"Spring 2025", std::chrono::year_month_day{2025y/1/18}, std::chrono::year_month_day{2025y/5/28}};
    ASSERT_FALSE(utils::isOnlyWhitespace(term2.getId()));
    ASSERT_EQ(term2.getTitle(), "Spring 2025");
    ASSERT_EQ(term2.getStartDate(), std::chrono::year_month_day{2025y/1/18});
    ASSERT_EQ(term2.getEndDate(), std::chrono::year_month_day{2025y/5/28});
}

TEST_F(TermTest, FourParamInitialization) {
    Term term2{"Spring 2025", std::chrono::year_month_day{2025y/1/18}, std::chrono::year_month_day{2025y/5/28}, false};
    ASSERT_FALSE(utils::isOnlyWhitespace(term2.getId()));
    ASSERT_EQ(term2.getTitle(), "Spring 2025");
    ASSERT_EQ(term2.getStartDate(), std::chrono::year_month_day{2025y/1/18});
    ASSERT_EQ(term2.getEndDate(), std::chrono::year_month_day{2025y/5/28});
    ASSERT_FALSE(term2.getActive());
}

// ====================================
// FUNCTION SMOKE TESTS
// ====================================

TEST_F(TermTest, PrintTermInfo) {
    std::stringstream ss;

    // initialize courses for totalCredits and ovrGpa
    Course course1{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course2{"ENGR 195A", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    term1.addCourse(course1);
    term1.addCourse(course2);

    term1.printTermInfo(ss);
    std::string output = ss.str();

    // find and remove the UUID part
    std::regex uuidRegex("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    output = std::regex_replace(output, uuidRegex, "<UUID>");

    ASSERT_EQ(output, "===========================================================\nID: <UUID>\nTerm: Fall 2025\n"
                        "Duration: 2025-08-12 - 2025-12-05\nTotal Credits: 6\nOverall GPA: 0\n"
                        "Current? No\n===========================================================\n");
}

TEST_F(TermTest, AddCourse) {
    Course course1{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};

    term1.addCourse(course1);
    ASSERT_EQ(term1.getCourseList().size(), 1);
}

TEST_F(TermTest, RemoveCourse) {
    Course course1{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course2{"ENGR 195A", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};

    term1.addCourse(course1);
    term1.addCourse(course2);
    std::string id = course1.getId();
    term1.removeCourse(id);

    // check size and success of removal
    ASSERT_EQ(term1.getCourseList().size(), 1);
    ASSERT_THROW(term1.getCourseList().at(id), std::out_of_range);
}

TEST_F(TermTest, FindCourseConst) {
    Course course1{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course2{"ENGR 195A", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};

    term1.addCourse(course1);
    term1.addCourse(course2);
    std::string id = course1.getId();
    
    // cast to const Term to use const version of function
    ASSERT_EQ(static_cast<const Term&>(term1).findCourse(id), course1);
}

TEST_F(TermTest, FindCourseNonConst) {
    Course course1{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course2{"ENGR 195A", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    Course course3 = course1;   // make course3 the same as course1

    term1.addCourse(course1);
    term1.addCourse(course2);
    std::string id = course1.getId();
    term1.findCourse(id).setActive(false);
    course3.setActive(false);   // copy change to course1

    ASSERT_EQ(term1.findCourse(id), course3);
}

TEST_F(TermTest, OverloadedEquals) {
    Term term2{"Spring 2025", std::chrono::year_month_day{2025y/1/18}, std::chrono::year_month_day{2025y/5/28}, false};
    Term term3{"Spring 2026", std::chrono::year_month_day{2026y/1/20}, std::chrono::year_month_day{2026y/5/23}};
    Term term4 = term1;

    ASSERT_FALSE(term1 == term2);
    ASSERT_FALSE(term1 == term3);
    ASSERT_TRUE(term1 == term4);
}

// ====================================
// GETTER EDGE CASES
// ====================================

TEST_F(TermTest, ActiveGetterEmpty) {
    Term term2{"Spring 2025", std::chrono::year_month_day{2025y/1/18}, std::chrono::year_month_day{2025y/5/28}};
    ASSERT_TRUE(term2.getActive());
}

// ====================================
// SETTER EDGE CASES
// ====================================

TEST_F(TermTest, TitleSetterInvalid) {
    // throw invalid argument since title is empty
    ASSERT_THROW(term1.setTitle(""), std::invalid_argument);
    ASSERT_EQ(term1.getTitle(), "Fall 2025");
}

TEST_F(TermTest, TitleSetterWhitespaceInvalid) {
    // throw invalid argument since title is whitespace
    ASSERT_THROW(term1.setTitle(" "), std::invalid_argument);
    ASSERT_EQ(term1.getTitle(), "Fall 2025");
}

TEST_F(TermTest, StartDateSetterInvalid) {
    // throw invalid argument since date does not exist
    ASSERT_THROW(term1.setStartDate(std::chrono::year_month_day{2025y/2/30}), std::invalid_argument);
    ASSERT_EQ(term1.getStartDate(), std::chrono::year_month_day{2025y/8/12});
}

TEST_F(TermTest, EndDateSetterInvalid) {
    // throw invalid argument since date does not exist
    ASSERT_THROW(term1.setEndDate(std::chrono::year_month_day{2025y/2/30}), std::invalid_argument);
    ASSERT_EQ(term1.getEndDate(), std::chrono::year_month_day{2025y/12/5});
}

// ====================================
// INITIALIZATION EDGE CASES
// ====================================

TEST_F(TermTest, ThreeParamInitializationNoTitle) {
    // throw invalid argument since title is empty
    ASSERT_THROW((Term{"", std::chrono::year_month_day{2025y/1/18}, std::chrono::year_month_day{2025y/5/28}}), std::invalid_argument);
}

TEST_F(TermTest, ThreeParamInitializationInvalidStartDate) {
    // throw invalid argument since start date does not exist
    ASSERT_THROW((Term{"Spring 2026", std::chrono::year_month_day{2026y/2/31}, std::chrono::year_month_day{2026y/5/28}}), std::invalid_argument);
}

TEST_F(TermTest, ThreeParamInitializationInvalidEndDate) {
    // throw invalid argument since end date does not exist
    ASSERT_THROW((Term{"Spring 2026", std::chrono::year_month_day{2026y/1/18}, std::chrono::year_month_day{2026y/2/31}}), std::invalid_argument);
}

// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST_F(TermTest, PrintTermInfoPartial) {
    std::stringstream ss;
    Term term2{"Spring 2025", std::chrono::year_month_day{2025y/1/18}, std::chrono::year_month_day{2025y/5/28}};
    Course course1{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    term2.addCourse(course1);
    term2.printTermInfo(ss);
    std::string output = ss.str();

    // find and remove the UUID part
    std::regex uuidRegex("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    output = std::regex_replace(output, uuidRegex, "<UUID>");

    ASSERT_EQ(output, "===========================================================\nID: <UUID>\nTerm: Spring 2025\n"
                        "Duration: 2025-01-18 - 2025-05-28\nTotal Credits: 3\nOverall GPA: 0\n"
                        "Current? Yes\n===========================================================\n");
}

TEST_F(TermTest, AddCourseAlreadyExists) {
    Course course1{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};

    term1.addCourse(course1);

    // throw logic error since Course already exists in list
    ASSERT_THROW(term1.addCourse(course1), std::logic_error);
}

TEST_F(TermTest, RemoveCourseNotFound) {
    Course course1{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course2{"ENGR 195A", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};
    std::string id = course2.getId();

    term1.addCourse(course1);

    // throw out of range since ID was not found
    ASSERT_THROW(term1.removeCourse(id), std::out_of_range);
}

TEST_F(TermTest, FindCourseConstNotFound) {
    Course course1{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course2{"ENGR 195A", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};

    term1.addCourse(course1);
    std::string id = course2.getId();

    // cast to const Term to use const version of function
    // throw out of range since ID was not found
    ASSERT_THROW(static_cast<const Term&>(term1).findCourse(id), std::out_of_range);
}

TEST_F(TermTest, FindCourseNonConstNotFound) {
    Course course1{"CMPE 142", "Operating Systems", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, 3, false};
    Course course2{"ENGR 195A", std::chrono::year_month_day{2025y/8/14}, std::chrono::year_month_day{2025y/12/18}};

    term1.addCourse(course1);
    std::string id = course2.getId();

    // cast to const Term to use const version of function
    // throw out of range since ID was not found
    ASSERT_THROW(term1.findCourse(id), std::out_of_range);
}

TEST_F(TermTest, OverloadedEqualsSameTitleDifferentParams) {
    Term term2{"Fall 2025", std::chrono::year_month_day{2025y/9/1}, std::chrono::year_month_day{2025y/12/5}, false};
    Term term3{"Fall 2025", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/11/29}, false};
    Term term4{"Fall 2025", std::chrono::year_month_day{2025y/8/12}, std::chrono::year_month_day{2025y/12/5}, true};

    ASSERT_FALSE(term1 == term2);
    ASSERT_FALSE(term1 == term3);
    ASSERT_FALSE(term1 == term4);
}

TEST_F(TermTest, OverloadedEqualsSameParamsDifferentId) {
    Term term2{"Fall 2025", std::chrono::year_month_day{2025y/9/1}, std::chrono::year_month_day{2025y/12/5}, false};
    Term term3{"Fall 2025", std::chrono::year_month_day{2025y/9/1}, std::chrono::year_month_day{2025y/12/5}, false};

    ASSERT_FALSE(term2 == term3);
}

// ====================================
// CLASS USE CASES
// ====================================