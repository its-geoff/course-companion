#include <gtest/gtest.h>
#include <chrono>
#include <string>
#include "model/Course.hpp"
#include "utils/utils.hpp"

using namespace std::chrono_literals;

// test fixture for class CourseRepository
class CourseRepositoryTest : public testing::Test {
    protected:
        const std::string validId    = "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
        const std::string validTitle = "CMPE 142";
        const std::string validDesc  = "Operating Systems";
        const std::chrono::year_month_day validStart{2025y/8/12};
        const std::chrono::year_month_day validEnd{2025y/12/5};
};

// ====================================
// SMOKE TESTS
// ====================================

TEST_F(CourseRepositoryTest, FromRowAllFields) {
    Course course = Course::fromRow(validId, validTitle, validDesc,
        validStart, validEnd, 3, false);

    ASSERT_EQ(course.getId(),          validId);
    ASSERT_EQ(course.getTitle(),       validTitle);
    ASSERT_EQ(course.getDescription(), validDesc);
    ASSERT_EQ(course.getStartDate(),   validStart);
    ASSERT_EQ(course.getEndDate(),     validEnd);
    ASSERT_EQ(course.getNumCredits(),  3);
    ASSERT_FALSE(course.getActive());
}

TEST_F(CourseRepositoryTest, FromRowPreservesId) {
    Course course = Course::fromRow(validId, validTitle, validDesc,
        validStart, validEnd, 3, false);

    ASSERT_EQ(course.getId(), validId);
}

TEST_F(CourseRepositoryTest, FromRowActiveTrue) {
    Course course = Course::fromRow(validId, validTitle, validDesc,
        validStart, validEnd, 3, true);

    ASSERT_TRUE(course.getActive());
}

TEST_F(CourseRepositoryTest, FromRowActiveFalse) {
    Course course = Course::fromRow(validId, validTitle, validDesc,
        validStart, validEnd, 3, false);

    ASSERT_FALSE(course.getActive());
}

TEST_F(CourseRepositoryTest, FromRowEmptyDescription) {
    Course course = Course::fromRow(validId, validTitle, "",
        validStart, validEnd, 3, false);

    ASSERT_EQ(course.getDescription(), "");
}

TEST_F(CourseRepositoryTest, FromRowTwoCoursesRetainSeparateIds) {
    std::string id1 = "550e8400-e29b-41d4-a716-446655440000";
    std::string id2 = "550e8400-e29b-41d4-a716-446655440001";

    Course course1 = Course::fromRow(id1, "CMPE 142", validDesc, validStart, validEnd, 3, false);
    Course course2 = Course::fromRow(id2, "ENGR 195A", "",
        std::chrono::year_month_day{2025y/8/14},
        std::chrono::year_month_day{2025y/12/18},
        1, true);

    ASSERT_EQ(course1.getId(), id1);
    ASSERT_EQ(course2.getId(), id2);
}

TEST_F(CourseRepositoryTest, FromRowDoesNotEqualNewCourse) {
    // a fromRow Course and a newly constructed Course with the same parameters should not be equal
    Course fromRowCourse = Course::fromRow(validId, validTitle, validDesc,
        validStart, validEnd, 3, false);
    Course newCourse{validTitle, validDesc, validStart, validEnd, 3, false};

    ASSERT_NE(fromRowCourse, newCourse);
}

// ====================================
// EDGE CASES
// ====================================

TEST_F(CourseRepositoryTest, FromRowEmptyTitle) {
    // throw invalid argument since title is empty
    ASSERT_THROW(Course::fromRow(validId, "", validDesc,
        validStart, validEnd, 3, false), std::invalid_argument);
}

TEST_F(CourseRepositoryTest, FromRowWhitespaceTitle) {
    // throw invalid argument since title is whitespace
    ASSERT_THROW(Course::fromRow(validId, "   ", validDesc,
        validStart, validEnd, 3, false), std::invalid_argument);
}

TEST_F(CourseRepositoryTest, FromRowEmptyStartDate) {
    // throw invalid argument since start date is empty
    ASSERT_THROW(Course::fromRow(validId, validTitle, validDesc,
        std::chrono::year_month_day{}, validEnd, 3, false), std::invalid_argument);
}

TEST_F(CourseRepositoryTest, FromRowEmptyEndDate) {
    // throw invalid argument since end date is empty
    ASSERT_THROW(Course::fromRow(validId, validTitle, validDesc,
        validStart, std::chrono::year_month_day{}, 3, false), std::invalid_argument);
}

TEST_F(CourseRepositoryTest, FromRowInvalidStartDate) {
    // throw invalid argument since start date does not exist
    ASSERT_THROW(Course::fromRow(validId, validTitle, validDesc,
        std::chrono::year_month_day{2025y/2/30}, validEnd, 3, false), std::invalid_argument);
}

TEST_F(CourseRepositoryTest, FromRowInvalidEndDate) {
    // throw invalid argument since end date does not exist
    ASSERT_THROW(Course::fromRow(validId, validTitle, validDesc,
        validStart, std::chrono::year_month_day{2025y/4/31}, 3, false), std::invalid_argument);
}

TEST_F(CourseRepositoryTest, FromRowEndDateBeforeStartDate) {
    // throw logic error since end date is before start date
    ASSERT_THROW(Course::fromRow(validId, validTitle, validDesc,
        std::chrono::year_month_day{2025y/12/5},
        std::chrono::year_month_day{2025y/8/12},
        3, false), std::logic_error);
}

TEST_F(CourseRepositoryTest, FromRowInvalidNumCredits) {
    // throw out of range since num credits is below 0
    ASSERT_THROW(Course::fromRow(validId, validTitle, validDesc,
        validStart, validEnd, -1, false), std::out_of_range);
}

TEST_F(CourseRepositoryTest, FromRowZeroCredits) {
    Course course = Course::fromRow(validId, validTitle, validDesc,
        validStart, validEnd, 0, false);

    ASSERT_EQ(course.getNumCredits(), 0);
}

TEST_F(CourseRepositoryTest, FromRowSameDayStartAndEnd) {
    Course course = Course::fromRow(validId, validTitle, validDesc,
        std::chrono::year_month_day{2025y/8/12},
        std::chrono::year_month_day{2025y/8/12},
        3, false);

    ASSERT_EQ(course.getStartDate(), course.getEndDate());
}

TEST_F(CourseRepositoryTest, FromRowYearBoundaryDates) {
    Course course = Course::fromRow(validId, validTitle, validDesc,
        std::chrono::year_month_day{2024y/12/31},
        std::chrono::year_month_day{2025y/1/1},
        3, false);

    ASSERT_EQ(course.getStartDate(), (std::chrono::year_month_day{2024y/12/31}));
    ASSERT_EQ(course.getEndDate(),   (std::chrono::year_month_day{2025y/1/1}));
}

TEST_F(CourseRepositoryTest, FromRowLeapYearStartDate) {
    Course course = Course::fromRow(validId, validTitle, validDesc,
        std::chrono::year_month_day{2024y/2/29},
        std::chrono::year_month_day{2024y/6/1},
        3, false);

    ASSERT_EQ(course.getStartDate(), (std::chrono::year_month_day{2024y/2/29}));
}