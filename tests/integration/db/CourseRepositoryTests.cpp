#include <gtest/gtest.h>
#include <chrono>
#include <string>
#include <cstdlib>
#include "db/DatabaseConnection.hpp"
#include "db/TermRepository.hpp"
#include "db/CourseRepository.hpp"
#include "model/Term.hpp"
#include "model/Course.hpp"

using namespace std::chrono_literals;

// test fixture for CourseRepository integration tests
class CourseRepositoryTest : public testing::Test {
    protected:
        std::unique_ptr<DatabaseConnection> db_;
        std::unique_ptr<TermRepository> termRepo_;
        std::unique_ptr<CourseRepository> repo_;

        // parent term inserted in SetUp; all courses belong to this term
        Term parentTerm_{"Fall 2025",
            std::chrono::year_month_day{2025y/8/12},
            std::chrono::year_month_day{2025y/12/5},
            false};

        const std::string validTitle = "CMPE 142";
        const std::string validDesc  = "Operating Systems";
        const std::chrono::year_month_day validStart{2025y/8/12};
        const std::chrono::year_month_day validEnd{2025y/12/5};

        void SetUp() override {
            db_ = std::make_unique<DatabaseConnection>(
                std::getenv("DB_HOST"),
                static_cast<unsigned int>(std::stoi(std::getenv("DB_PORT"))),
                std::getenv("DB_USER"),
                std::getenv("DB_PASSWORD"),
                std::getenv("TEST_DB_SCHEMA")
            );
            termRepo_ = std::make_unique<TermRepository>(*db_);
            repo_     = std::make_unique<CourseRepository>(*db_, parentTerm_.getId());

            db_->getSession().startTransaction();
            termRepo_->insert(parentTerm_);
        }

        void TearDown() override {
            db_->getSession().rollback();
        }
};

// ====================================
// SMOKE TESTS
// ====================================

TEST_F(CourseRepositoryTest, InsertAndFindById) {
    Course course{validTitle, validDesc, validStart, validEnd, 3, false};
    repo_->insert(course);

    auto result = repo_->findById(course.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getId(),          course.getId());
    ASSERT_EQ(result->getTitle(),       validTitle);
    ASSERT_EQ(result->getDescription(), validDesc);
    ASSERT_EQ(result->getStartDate(),   validStart);
    ASSERT_EQ(result->getEndDate(),     validEnd);
    ASSERT_EQ(result->getNumCredits(),  3);
    ASSERT_FALSE(result->getActive());
}

TEST_F(CourseRepositoryTest, InsertAndFindAll) {
    Course course1{validTitle, validDesc, validStart, validEnd, 3, false};
    Course course2{"ENGR 195A", "",
        std::chrono::year_month_day{2025y/8/14},
        std::chrono::year_month_day{2025y/12/18},
        1, true};

    repo_->insert(course1);
    repo_->insert(course2);

    auto results = repo_->findAll();

    ASSERT_GE(results.size(), 2u);
}

TEST_F(CourseRepositoryTest, InsertAndFindByParentId) {
    Course course1{validTitle, validDesc, validStart, validEnd, 3, false};
    Course course2{"ENGR 195A", "", validStart, validEnd, 1, true};

    repo_->insert(course1);
    repo_->insert(course2);

    auto results = repo_->findByParentId(parentTerm_.getId());

    ASSERT_EQ(results.size(), 2u);
}

TEST_F(CourseRepositoryTest, InsertActiveTrue) {
    Course course{validTitle, validDesc, validStart, validEnd, 3, true};
    repo_->insert(course);

    auto result = repo_->findById(course.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result->getActive());
}

TEST_F(CourseRepositoryTest, InsertActiveFalse) {
    Course course{validTitle, validDesc, validStart, validEnd, 3, false};
    repo_->insert(course);

    auto result = repo_->findById(course.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_FALSE(result->getActive());
}

TEST_F(CourseRepositoryTest, InsertEmptyDescription) {
    Course course{validTitle, "", validStart, validEnd, 3, false};
    repo_->insert(course);

    auto result = repo_->findById(course.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getDescription(), "");
}

TEST_F(CourseRepositoryTest, UpdateTitle) {
    Course course{validTitle, validDesc, validStart, validEnd, 3, false};
    repo_->insert(course);

    course.setTitle("CMPE 180");
    repo_->update(course);

    auto result = repo_->findById(course.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getTitle(), "CMPE 180");
}

TEST_F(CourseRepositoryTest, UpdateDescription) {
    Course course{validTitle, validDesc, validStart, validEnd, 3, false};
    repo_->insert(course);

    course.setDescription("Updated description.");
    repo_->update(course);

    auto result = repo_->findById(course.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getDescription(), "Updated description.");
}

TEST_F(CourseRepositoryTest, UpdateNumCredits) {
    Course course{validTitle, validDesc, validStart, validEnd, 3, false};
    repo_->insert(course);

    course.setNumCredits(4);
    repo_->update(course);

    auto result = repo_->findById(course.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getNumCredits(), 4);
}

TEST_F(CourseRepositoryTest, UpdateActive) {
    Course course{validTitle, validDesc, validStart, validEnd, 3, false};
    repo_->insert(course);

    course.setActive(true);
    repo_->update(course);

    auto result = repo_->findById(course.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result->getActive());
}

TEST_F(CourseRepositoryTest, RemoveCourse) {
    Course course{validTitle, validDesc, validStart, validEnd, 3, false};
    repo_->insert(course);

    repo_->remove(course.getId());

    auto result = repo_->findById(course.getId());

    ASSERT_FALSE(result.has_value());
}

TEST_F(CourseRepositoryTest, FindByIdNotFound) {
    auto result = repo_->findById("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee");

    ASSERT_FALSE(result.has_value());
}

TEST_F(CourseRepositoryTest, FindAllEmpty) {
    auto results = repo_->findAll();

    ASSERT_EQ(results.size(), 0u);
}

TEST_F(CourseRepositoryTest, FindByParentIdEmpty) {
    auto results = repo_->findByParentId(parentTerm_.getId());

    ASSERT_EQ(results.size(), 0u);
}

// ====================================
// EDGE CASES
// ====================================

TEST_F(CourseRepositoryTest, InsertDuplicateTitleUnderSameTermThrows) {
    Course course1{validTitle, validDesc, validStart, validEnd, 3, false};
    Course course2{validTitle, "Different desc", validStart, validEnd, 4, true};

    repo_->insert(course1);

    // duplicate title under same term_id violates UNIQUE (term_id, title)
    ASSERT_THROW(repo_->insert(course2), std::exception);
}

TEST_F(CourseRepositoryTest, InsertInvalidTermIdThrows) {
    CourseRepository orphanRepo{*db_, "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee"};
    Course course{validTitle, validDesc, validStart, validEnd, 3, false};

    // foreign key violation: term id does not exist
    ASSERT_THROW(orphanRepo.insert(course), std::exception);
}

TEST_F(CourseRepositoryTest, FindByParentIdOnlyReturnsCoursesBelongingToTerm) {
    // second term with its own course
    Term otherTerm{"Spring 2026",
        std::chrono::year_month_day{2026y/1/20},
        std::chrono::year_month_day{2026y/5/23},
        true};
    termRepo_->insert(otherTerm);

    CourseRepository otherRepo{*db_, otherTerm.getId()};
    Course otherCourse{"CMPE 180", "", validStart, validEnd, 3, false};
    otherRepo.insert(otherCourse);

    Course parentCourse{validTitle, validDesc, validStart, validEnd, 3, false};
    repo_->insert(parentCourse);

    auto results = repo_->findByParentId(parentTerm_.getId());

    ASSERT_EQ(results.size(), 1u);
    ASSERT_EQ(results[0].getTitle(), validTitle);
}

TEST_F(CourseRepositoryTest, InsertRoundTripsId) {
    Course course{validTitle, validDesc, validStart, validEnd, 3, false};
    repo_->insert(course);

    auto result = repo_->findById(course.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getId(), course.getId());
}

TEST_F(CourseRepositoryTest, InsertZeroCredits) {
    Course course{validTitle, validDesc, validStart, validEnd, 0, false};
    repo_->insert(course);

    auto result = repo_->findById(course.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getNumCredits(), 0);
}

TEST_F(CourseRepositoryTest, InsertYearBoundaryDates) {
    Course course{validTitle, validDesc,
        std::chrono::year_month_day{2024y/12/31},
        std::chrono::year_month_day{2025y/1/1},
        3, false};
    repo_->insert(course);

    auto result = repo_->findById(course.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getStartDate(), (std::chrono::year_month_day{2024y/12/31}));
    ASSERT_EQ(result->getEndDate(),   (std::chrono::year_month_day{2025y/1/1}));
}

TEST_F(CourseRepositoryTest, InsertLeapYearStartDate) {
    Course course{validTitle, validDesc,
        std::chrono::year_month_day{2024y/2/29},
        std::chrono::year_month_day{2024y/6/1},
        3, false};
    repo_->insert(course);

    auto result = repo_->findById(course.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getStartDate(), (std::chrono::year_month_day{2024y/2/29}));
}

TEST_F(CourseRepositoryTest, RemoveNonExistentIdNoThrow) {
    ASSERT_NO_THROW(repo_->remove("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee"));
}

TEST_F(CourseRepositoryTest, CascadeDeleteRemovesCourseWithTerm) {
    Course course{validTitle, validDesc, validStart, validEnd, 3, false};
    repo_->insert(course);

    termRepo_->remove(parentTerm_.getId());

    auto result = repo_->findById(course.getId());

    // course should be gone due to ON DELETE CASCADE
    ASSERT_FALSE(result.has_value());
}