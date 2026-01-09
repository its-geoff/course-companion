#include <gtest/gtest.h>
#include <sstream>      // diverts output from terminal to separate stream
#include <regex>        // regular expression matching for UUIDs
#include "controller/CourseController.hpp"
#include "utils/utils.hpp"

using namespace std::chrono_literals;

// test fixture for class CourseController
class CourseControllerTest : public testing::Test {
    protected:
        Term term{"Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false};
        CourseController controller{term};
};

// ====================================
// GETTER SMOKE TESTS
// ====================================

TEST_F(CourseControllerTest, CourseListGetter) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    controller.addCourse("CMPE 142", "Operating Systems", {}, {}, 3, true);

    const std::unordered_map<std::string, Course> &listOfCourses = controller.getCourseList();
    ASSERT_EQ(listOfCourses.size(), 2);

    // check if both added courses are in the list
    std::string id1 = controller.getCourseId("ENGR 195A");
    auto it1 = listOfCourses.find(id1);
    ASSERT_TRUE(it1 != listOfCourses.end());

    std::string id2 = controller.getCourseId("CMPE 142");
    auto it2 = listOfCourses.find(id2);
    ASSERT_TRUE(it2 != listOfCourses.end());
}

TEST_F(CourseControllerTest, CourseIdGetter) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    controller.addCourse("CMPE 142", "Operating Systems", {}, {}, 3, true);

    std::string id = controller.getCourseId("ENGR 195A");

    // find and replace UUID
    std::regex uuidRegex("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    id = std::regex_replace(id, uuidRegex, "<UUID>");

    // if the UUID is in the correct format, it should match this
    ASSERT_EQ(id, "<UUID>");
}

// ====================================
// FUNCTION SMOKE TESTS
// ====================================

TEST_F(CourseControllerTest, AddCourse) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);

    const Course& selectedCourse = controller.findCourse("ENGR 195A");
    ASSERT_EQ(selectedCourse.getTitle(), "ENGR 195A");
    ASSERT_EQ(selectedCourse.getDescription(), "");
    ASSERT_EQ(selectedCourse.getStartDate(), std::chrono::year_month_day{2026y/1/2});
    ASSERT_EQ(selectedCourse.getEndDate(), std::chrono::year_month_day{2026y/5/12});
    ASSERT_EQ(selectedCourse.getNumCredits(), 3);
    ASSERT_FALSE(selectedCourse.getActive());
}

TEST_F(CourseControllerTest, EditTitle) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);

    std::string id = controller.getCourseId("ENGR 195A");
    controller.editTitle(id, "CMPE 152");

    // check that title has been edited and title -> id mapping is correct
    const Course& selectedCourse = controller.findCourse("CMPE 152");
    ASSERT_EQ(selectedCourse.getTitle(), "CMPE 152");
    ASSERT_EQ(controller.getCourseId("CMPE 152"), id);
}

TEST_F(CourseControllerTest, EditDescription) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);

    std::string id = controller.getCourseId("ENGR 195A");
    controller.editDescription(id, "Global and Social Issues in Engineering");

    const Course& selectedCourse = controller.findCourse("ENGR 195A");
    ASSERT_EQ(selectedCourse.getDescription(), "Global and Social Issues in Engineering");
}

TEST_F(CourseControllerTest, EditStartDate) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);


    std::string id = controller.getCourseId("ENGR 195A");
    controller.editStartDate(id, std::chrono::year_month_day{2026y/2/11});

    const Course& selectedCourse = controller.findCourse("ENGR 195A");
    ASSERT_EQ(selectedCourse.getStartDate(), std::chrono::year_month_day{2026y/2/11});
}

TEST_F(CourseControllerTest, EditEndDate) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);


    std::string id = controller.getCourseId("ENGR 195A");
    controller.editEndDate(id, std::chrono::year_month_day{2026y/5/30});

    const Course& selectedCourse = controller.findCourse("ENGR 195A");
    ASSERT_EQ(selectedCourse.getEndDate(), std::chrono::year_month_day{2026y/5/30});
}

TEST_F(CourseControllerTest, EditNumCredits) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);


    std::string id = controller.getCourseId("ENGR 195A");
    controller.editNumCredits(id, 4);

    const Course& selectedCourse = controller.findCourse("ENGR 195A");
    ASSERT_EQ(selectedCourse.getNumCredits(), 4);
}

TEST_F(CourseControllerTest, EditActive) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);


    std::string id = controller.getCourseId("ENGR 195A");
    controller.editActive(id, true);

    const Course& selectedCourse = controller.findCourse("ENGR 195A");
    ASSERT_TRUE(selectedCourse.getActive());
}

TEST_F(CourseControllerTest, RemoveCourse) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    controller.addCourse("CMPE 142", "Operating Systems", {}, {}, 3, true);

    controller.removeCourse("ENGR 195A");

    const std::unordered_map<std::string, Course>& listOfCourses = controller.getCourseList();
    ASSERT_EQ(listOfCourses.size(), 1);

    // throw out of range since the course is not in the list
    ASSERT_THROW(controller.getCourseId("ENGR 195A"), std::out_of_range);

    std::string id2 = controller.getCourseId("CMPE 142");
    auto it2 = listOfCourses.find(id2);
    ASSERT_TRUE(it2 != listOfCourses.end());
}

TEST_F(CourseControllerTest, FindCourseConst) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    controller.addCourse("CMPE 142", "Operating Systems", {}, {}, 3, true);

    const CourseController& constController = controller;
    const Course& selectedCourse = constController.findCourse("CMPE 142");
    ASSERT_EQ(selectedCourse.getTitle(), "CMPE 142");
    ASSERT_EQ(selectedCourse.getDescription(), "Operating Systems");
    ASSERT_EQ(selectedCourse.getStartDate(), utils::getTodayDate());
    ASSERT_EQ(selectedCourse.getEndDate(), (utils::getTodayDate() + std::chrono::months{4}));
    ASSERT_EQ(selectedCourse.getNumCredits(), 3);
    ASSERT_TRUE(selectedCourse.getActive());
}

TEST_F(CourseControllerTest, FindCourseNonConst) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    controller.addCourse("CMPE 142", "Operating Systems", {}, {}, 3, true);

    const Course& selectedCourse = controller.findCourse("CMPE 142");
    ASSERT_EQ(selectedCourse.getTitle(), "CMPE 142");
    ASSERT_EQ(selectedCourse.getDescription(), "Operating Systems");
    ASSERT_EQ(selectedCourse.getStartDate(), utils::getTodayDate());
    ASSERT_EQ(selectedCourse.getEndDate(), (utils::getTodayDate() + std::chrono::months{4}));
    ASSERT_EQ(selectedCourse.getNumCredits(), 3);
    ASSERT_TRUE(selectedCourse.getActive());
}

// ====================================
// GETTER EDGE CASES
// ====================================

TEST_F(CourseControllerTest, CourseListGetterEmpty) {
    const std::unordered_map<std::string, Course>& listOfCourses = controller.getCourseList();
    ASSERT_EQ(listOfCourses.size(), 0);
}

TEST_F(CourseControllerTest, CourseIdGetterNotFound) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    controller.addCourse("CMPE 142", "Operating Systems", {}, {}, 3, true);

    // out of range error since course cannot be found
    ASSERT_THROW(controller.getCourseId("CMPE 152"), std::out_of_range);
}

// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST_F(CourseControllerTest, AddTermAlreadyExists) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    // logic error since course already exists
    ASSERT_THROW(controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false), std::logic_error);
}

TEST_F(CourseControllerTest, EditTitleAlreadyExists) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    std::string id = controller.getCourseId("ENGR 195A");

    // logic error since term alraedy exists (title not changed)
    ASSERT_THROW(controller.editTitle(id, "ENGR 195A"), std::logic_error);
}

TEST_F(CourseControllerTest, EditTitleAlreadyExistsDifferentCase) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    std::string id = controller.getCourseId("ENGR 195A");

    // logic error since term alraedy exists (title not changed)
    ASSERT_THROW(controller.editTitle(id, "engr 195A"), std::logic_error);
}

TEST_F(CourseControllerTest, EditTitleEmpty) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    std::string id = controller.getCourseId("ENGR 195A");

    // invalid argument since title is empty
    ASSERT_THROW(controller.editTitle(id, ""), std::invalid_argument);
}

TEST_F(CourseControllerTest, EditStartDateInvalid) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    std::string id = controller.getCourseId("ENGR 195A");

    // invalid argument since start date doesn't exist
    ASSERT_THROW(controller.editStartDate(id, std::chrono::year_month_day{2026y/3/33}), std::invalid_argument);
}

TEST_F(CourseControllerTest, EditEndDateInvalid) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    std::string id = controller.getCourseId("ENGR 195A");

    // invalid argument since start date doesn't exist
    ASSERT_THROW(controller.editEndDate(id, std::chrono::year_month_day{2026y/3/33}), std::invalid_argument);
}

TEST_F(CourseControllerTest, EditNumCreditsLessThanZero) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    std::string id = controller.getCourseId("ENGR 195A");

    // out of range since numCredits must be greater than or equal to 0
    ASSERT_THROW(controller.editNumCredits(id, -3), std::out_of_range);
}

TEST_F(CourseControllerTest, RemoveCourseNotFound) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    controller.addCourse("CMPE 142", "Operating Systems", {}, {}, 3, true);

    // out of range since course cannot be found
    ASSERT_THROW(controller.removeCourse("CMPE 152"), std::out_of_range);
}

TEST_F(CourseControllerTest, FindCourseConstNotFound) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    controller.addCourse("CMPE 142", "Operating Systems", {}, {}, 3, true);

    // out of range error since course cannot be found
    const CourseController& constController = controller;
    ASSERT_THROW(constController.findCourse("CMPE 152"), std::out_of_range);
}

TEST_F(CourseControllerTest, FindCourseNonConstNotFound) {
    controller.addCourse("ENGR 195A", "", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/12}, 3, false);
    controller.addCourse("CMPE 142", "Operating Systems", {}, {}, 3, true);

    // out of range error since course cannot be found
    ASSERT_THROW(controller.findCourse("CMPE 152"), std::out_of_range);
}