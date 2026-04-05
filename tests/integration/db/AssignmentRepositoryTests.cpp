#include <gtest/gtest.h>
#include <chrono>
#include <string>
#include <cstdlib>
#include "db/DatabaseConnection.hpp"
#include "db/TermRepository.hpp"
#include "db/CourseRepository.hpp"
#include "db/AssignmentRepository.hpp"
#include "model/Term.hpp"
#include "model/Course.hpp"
#include "model/Assignment.hpp"

using namespace std::chrono_literals;

// test fixture for AssignmentRepository integration tests
class AssignmentRepositoryTest : public testing::Test {
    protected:
        std::unique_ptr<DatabaseConnection> db_;
        std::unique_ptr<TermRepository> termRepo_;
        std::unique_ptr<CourseRepository> courseRepo_;
        std::unique_ptr<AssignmentRepository> repo_;

        // parent term and course inserted in SetUp; all assignments belong to this course
        Term parentTerm_{"Fall 2025",
            std::chrono::year_month_day{2025y/8/12},
            std::chrono::year_month_day{2025y/12/5},
            false};
        Course parentCourse_{"CMPE 142", "Operating Systems",
            std::chrono::year_month_day{2025y/8/12},
            std::chrono::year_month_day{2025y/12/5},
            3, false};

        const std::string validTitle    = "Homework 3";
        const std::string validDesc     = "Focus on variables and strings.";
        const std::string validCategory = "Homework";
        const std::chrono::year_month_day validDueDate{2025y/11/20};

        void SetUp() override {
            const char* host     = std::getenv("DB_HOST");
            const char* port_str = std::getenv("DB_PORT");
            const char* user     = std::getenv("DB_USER");
            const char* password = std::getenv("DB_PASSWORD");
            const char* schema   = std::getenv("TEST_DB_SCHEMA");

            if (!host || !port_str || !user || !password || !schema) {
                GTEST_SKIP() << "Database environment variables not set.";
            }

            db_ = std::make_unique<DatabaseConnection>(
                host,
                static_cast<unsigned int>(std::stoi(port_str)),
                user,
                password,
                schema
            );

            termRepo_   = std::make_unique<TermRepository>(*db_);
            courseRepo_ = std::make_unique<CourseRepository>(*db_, parentTerm_.getId());
            repo_       = std::make_unique<AssignmentRepository>(*db_, parentCourse_.getId());

            db_->getSession().startTransaction();
            termRepo_->insert(parentTerm_);
            courseRepo_->insert(parentCourse_);
        }

        void TearDown() override {
            if (db_) {
                db_->getSession().rollback();
            }
        }
};

// ====================================
// SMOKE TESTS
// ====================================

TEST_F(AssignmentRepositoryTest, InsertAndFindById) {
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, true, 95.18f};
    repo_->insert(assignment);

    auto result = repo_->findById(assignment.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getId(),          assignment.getId());
    ASSERT_EQ(result->getTitle(),       validTitle);
    ASSERT_EQ(result->getDescription(), validDesc);
    ASSERT_EQ(result->getCategory(),    validCategory);
    ASSERT_EQ(result->getDueDate(),     validDueDate);
    ASSERT_TRUE(result->getCompleted());
    ASSERT_FLOAT_EQ(result->getGrade(), 95.18f);
}

TEST_F(AssignmentRepositoryTest, InsertAndFindAll) {
    Assignment assignment1{validTitle, validDesc, validCategory, validDueDate, true, 95.18f};
    Assignment assignment2{"Midterm", "", "Midterm",
        std::chrono::year_month_day{2025y/10/15}, true, 88.0f};

    repo_->insert(assignment1);
    repo_->insert(assignment2);

    auto results = repo_->findAll();

    ASSERT_GE(results.size(), 2u);
}

TEST_F(AssignmentRepositoryTest, InsertAndFindByParentId) {
    Assignment assignment1{validTitle, validDesc, validCategory, validDueDate, true, 95.18f};
    Assignment assignment2{"Midterm", "", "Midterm",
        std::chrono::year_month_day{2025y/10/15}, true, 88.0f};

    repo_->insert(assignment1);
    repo_->insert(assignment2);

    auto results = repo_->findByParentId(parentCourse_.getId());

    ASSERT_EQ(results.size(), 2u);
}

TEST_F(AssignmentRepositoryTest, InsertCompletedTrue) {
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, true, 95.18f};
    repo_->insert(assignment);

    auto result = repo_->findById(assignment.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result->getCompleted());
}

TEST_F(AssignmentRepositoryTest, InsertCompletedFalse) {
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, false, 0.0f};
    repo_->insert(assignment);

    auto result = repo_->findById(assignment.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_FALSE(result->getCompleted());
}

TEST_F(AssignmentRepositoryTest, InsertEmptyDescription) {
    Assignment assignment{validTitle, "", validCategory, validDueDate, false, 0.0f};
    repo_->insert(assignment);

    auto result = repo_->findById(assignment.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getDescription(), "");
}

TEST_F(AssignmentRepositoryTest, UpdateTitle) {
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, false, 0.0f};
    repo_->insert(assignment);

    assignment.setTitle("Homework 4");
    repo_->update(assignment);

    auto result = repo_->findById(assignment.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getTitle(), "Homework 4");
}

TEST_F(AssignmentRepositoryTest, UpdateDescription) {
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, false, 0.0f};
    repo_->insert(assignment);

    assignment.setDescription("Updated description.");
    repo_->update(assignment);

    auto result = repo_->findById(assignment.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getDescription(), "Updated description.");
}

TEST_F(AssignmentRepositoryTest, UpdateGrade) {
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, true, 80.0f};
    repo_->insert(assignment);

    assignment.setGrade(92.5f);
    repo_->update(assignment);

    auto result = repo_->findById(assignment.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_FLOAT_EQ(result->getGrade(), 92.5f);
}

TEST_F(AssignmentRepositoryTest, UpdateCompleted) {
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, false, 0.0f};
    repo_->insert(assignment);

    assignment.setCompleted(true);
    assignment.setGrade(88.0f);
    repo_->update(assignment);

    auto result = repo_->findById(assignment.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result->getCompleted());
    ASSERT_FLOAT_EQ(result->getGrade(), 88.0f);
}

TEST_F(AssignmentRepositoryTest, UpdateDueDate) {
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, false, 0.0f};
    repo_->insert(assignment);

    std::chrono::year_month_day newDue{2025y/12/1};
    assignment.setDueDate(newDue);
    repo_->update(assignment);

    auto result = repo_->findById(assignment.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getDueDate(), newDue);
}

TEST_F(AssignmentRepositoryTest, RemoveAssignment) {
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, false, 0.0f};
    repo_->insert(assignment);

    repo_->remove(assignment.getId());

    auto result = repo_->findById(assignment.getId());

    ASSERT_FALSE(result.has_value());
}

TEST_F(AssignmentRepositoryTest, FindByIdNotFound) {
    auto result = repo_->findById("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee");

    ASSERT_FALSE(result.has_value());
}

TEST_F(AssignmentRepositoryTest, FindAllEmpty) {
    auto results = repo_->findAll();

    ASSERT_EQ(results.size(), 0u);
}

TEST_F(AssignmentRepositoryTest, FindByParentIdEmpty) {
    auto results = repo_->findByParentId(parentCourse_.getId());

    ASSERT_EQ(results.size(), 0u);
}

// ====================================
// EDGE CASES
// ====================================

TEST_F(AssignmentRepositoryTest, InsertDuplicateTitleUnderSameCourseThrows) {
    Assignment assignment1{validTitle, validDesc, validCategory, validDueDate, false, 0.0f};
    Assignment assignment2{validTitle, "", validCategory, validDueDate, false, 0.0f};

    repo_->insert(assignment1);

    // duplicate title under same course_id violates UNIQUE (course_id, title)
    ASSERT_THROW(repo_->insert(assignment2), std::exception);
}

TEST_F(AssignmentRepositoryTest, InsertInvalidCourseIdThrows) {
    AssignmentRepository orphanRepo{*db_, "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee"};
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, false, 0.0f};

    // foreign key violation: course id does not exist
    ASSERT_THROW(orphanRepo.insert(assignment), std::exception);
}

TEST_F(AssignmentRepositoryTest, FindByParentIdOnlyReturnsAssignmentsBelongingToCourse) {
    // second course with its own assignment
    Course otherCourse{"ENGR 195A", "", 
        std::chrono::year_month_day{2025y/8/14},
        std::chrono::year_month_day{2025y/12/18},
        1, true};
    courseRepo_->insert(otherCourse);

    AssignmentRepository otherRepo{*db_, otherCourse.getId()};
    Assignment otherAssignment{"Final Report", "", "Final Exam",
        std::chrono::year_month_day{2025y/12/10}, false, 0.0f};
    otherRepo.insert(otherAssignment);

    Assignment parentAssignment{validTitle, validDesc, validCategory, validDueDate, false, 0.0f};
    repo_->insert(parentAssignment);

    auto results = repo_->findByParentId(parentCourse_.getId());

    ASSERT_EQ(results.size(), 1u);
    ASSERT_EQ(results[0].getTitle(), validTitle);
}

TEST_F(AssignmentRepositoryTest, InsertRoundTripsId) {
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, false, 0.0f};
    repo_->insert(assignment);

    auto result = repo_->findById(assignment.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getId(), assignment.getId());
}

TEST_F(AssignmentRepositoryTest, InsertGradeBoundaryLow) {
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, true, 0.0f};
    repo_->insert(assignment);

    auto result = repo_->findById(assignment.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_FLOAT_EQ(result->getGrade(), 0.0f);
}

TEST_F(AssignmentRepositoryTest, InsertGradeBoundaryHigh) {
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, true, 150.0f};
    repo_->insert(assignment);

    auto result = repo_->findById(assignment.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_FLOAT_EQ(result->getGrade(), 150.0f);
}

TEST_F(AssignmentRepositoryTest, InsertLeapYearDueDate) {
    Assignment assignment{validTitle, validDesc, validCategory,
        std::chrono::year_month_day{2024y/2/29}, false, 0.0f};
    repo_->insert(assignment);

    auto result = repo_->findById(assignment.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getDueDate(), (std::chrono::year_month_day{2024y/2/29}));
}

TEST_F(AssignmentRepositoryTest, RemoveNonExistentIdNoThrow) {
    ASSERT_NO_THROW(repo_->remove("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee"));
}

TEST_F(AssignmentRepositoryTest, CascadeDeleteRemovesAssignmentWithCourse) {
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, false, 0.0f};
    repo_->insert(assignment);

    courseRepo_->remove(parentCourse_.getId());

    auto result = repo_->findById(assignment.getId());

    // assignment should be gone due to ON DELETE CASCADE
    ASSERT_FALSE(result.has_value());
}

TEST_F(AssignmentRepositoryTest, CascadeDeleteRemovesAssignmentWithTerm) {
    Assignment assignment{validTitle, validDesc, validCategory, validDueDate, false, 0.0f};
    repo_->insert(assignment);

    termRepo_->remove(parentTerm_.getId());

    auto result = repo_->findById(assignment.getId());

    // assignment should be gone due to cascading delete through course
    ASSERT_FALSE(result.has_value());
}