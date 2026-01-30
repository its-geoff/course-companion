#include <gtest/gtest.h>
#include "model/Course.hpp"
#include "model/Term.hpp"

using namespace std::chrono_literals;

TEST(TermCourseIntegrationTest, AddCourseToTerm) {
    Term term1{"Spring 2025", {}, {}};
    Course course1{"CMPE 152", "", {}, {}};

    term1.addCourse(course1);

    // check that course was added
    ASSERT_EQ(term1.getCourseList().size(), 1);
}

TEST(TermCourseIntegrationTest, GradeCalculationAcrossTerm) {
    Term term1{"Spring 2025", {}, {}};
    Course course1{"CMPE 152", "", {}, {}};
    Course course2{"CMPE 142", "", {}, {}};
    Course course3{"ENGR 195A", "", {}, {}, 1};

    course1.setGradePct(84.0f);
    course1.setGpaVal();
    course2.setGradePct(90.0f);
    course2.setGpaVal();
    course3.setGradePct(88.8f);
    course3.setGpaVal();
    term1.addCourse(course1);
    term1.addCourse(course2);
    term1.addCourse(course3);

    ASSERT_FLOAT_EQ(term1.getOvrGpa(), 3.34f);
}

TEST(TermCourseIntegrationTest, RemoveCourseFromTerm) {
    Term term1{"Spring 2025", {}, {}};
    Course course1{"CMPE 152", "", {}, {}};

    term1.addCourse(course1);
    term1.removeCourse(course1.getId());

    // no courses left in list
    ASSERT_TRUE(term1.getCourseList().empty());
}

TEST(TermCourseIntegrationTest, EmptyTermReturnsGpa) {
    Term term1{"Spring 2025", {}, {}};

    ASSERT_EQ(term1.getCourseList().size(), 0);
    ASSERT_FLOAT_EQ(term1.getOvrGpa(), 0.0f);
}

TEST(TermCourseIntegrationTest, CourseUpdatesShownInTerm) {
    Term term1{"Spring 2025", {}, {}};
    Course course1{"CMPE 152", "", {}, {}};

    // check initial values
    ASSERT_FLOAT_EQ(course1.getGradePct(), 0.0f);
    ASSERT_FLOAT_EQ(term1.getOvrGpa(), 0.0f);

    course1.setGradePct(82.0f);
    course1.setGpaVal();
    term1.addCourse(course1);

    // check edited values
    ASSERT_FLOAT_EQ(course1.getGradePct(), 82.0f);
    ASSERT_FLOAT_EQ(term1.getOvrGpa(), 2.7f);
}

TEST(TermCourseIntegrationTest, CoursesWithNoGrades) {
    Term term{"Spring 2025", {}, {}};
    Course course1{"CMPE 152", "", {}, {}};
    Course course2{"CMPE 142", "", {}, {}};

    term.addCourse(course1);
    term.addCourse(course2);

    // GPA should be default
    ASSERT_FLOAT_EQ(term.getOvrGpa(), 0.0f);
}

TEST(TermCourseIntegrationTest, RemoveNonexistentCourse) {
    Term term{"Spring 2025", {}, {}};

    ASSERT_THROW(term.removeCourse("nonexistent-course-id"), std::out_of_range);
    ASSERT_TRUE(term.getCourseList().empty());
}

TEST(TermCourseIntegrationTest, MultipleCourseUpdates) {
    Term term{"Spring 2025", {}, {}};
    Course course1{"CMPE 152", "", {}, {}};
    Course course2{"CMPE 142", "", {}, {}};

    course1.setGradePct(80.0f);
    course1.setGpaVal();
    course2.setGradePct(90.0f);
    course2.setGpaVal();
    term.addCourse(course1);
    term.addCourse(course2);
    ASSERT_FLOAT_EQ(term.getOvrGpa(), 3.2f);

    term.removeCourse(course1.getId());
    // GPA with only course2
    ASSERT_FLOAT_EQ(term.getOvrGpa(), 3.7f);
}
