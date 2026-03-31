#include <gtest/gtest.h>
#include <chrono>
#include <string>
#include <cstdlib>
#include "db/DatabaseConnection.hpp"
#include "db/TermRepository.hpp"
#include "model/Term.hpp"

using namespace std::chrono_literals;

// test fixture for TermRepository integration tests
class TermRepositoryTest : public testing::Test {
    protected:
        std::unique_ptr<DatabaseConnection> db_;
        std::unique_ptr<TermRepository> repo_;

        const std::string validTitle = "Fall 2025";
        const std::chrono::year_month_day validStart{2025y/8/12};
        const std::chrono::year_month_day validEnd{2025y/12/5};

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

            repo_ = std::make_unique<TermRepository>(*db_);
            db_->getSession().startTransaction();
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

TEST_F(TermRepositoryTest, InsertAndFindById) {
    Term term{validTitle, validStart, validEnd, false};
    repo_->insert(term);

    auto result = repo_->findById(term.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getId(),        term.getId());
    ASSERT_EQ(result->getTitle(),     validTitle);
    ASSERT_EQ(result->getStartDate(), validStart);
    ASSERT_EQ(result->getEndDate(),   validEnd);
    ASSERT_FALSE(result->getActive());
}

TEST_F(TermRepositoryTest, InsertAndFindAll) {
    Term term1{"Fall 2025", validStart, validEnd, false};
    Term term2{"Spring 2026",
        std::chrono::year_month_day{2026y/1/20},
        std::chrono::year_month_day{2026y/5/23},
        true};

    repo_->insert(term1);
    repo_->insert(term2);

    auto results = repo_->findAll();

    // at least the two inserted terms are present
    ASSERT_GE(results.size(), 2u);
}

TEST_F(TermRepositoryTest, InsertActiveTrue) {
    Term term{validTitle, validStart, validEnd, true};
    repo_->insert(term);

    auto result = repo_->findById(term.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result->getActive());
}

TEST_F(TermRepositoryTest, InsertActiveFalse) {
    Term term{validTitle, validStart, validEnd, false};
    repo_->insert(term);

    auto result = repo_->findById(term.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_FALSE(result->getActive());
}

TEST_F(TermRepositoryTest, UpdateTitle) {
    Term term{validTitle, validStart, validEnd, false};
    repo_->insert(term);

    term.setTitle("Spring 2026");
    repo_->update(term);

    auto result = repo_->findById(term.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getTitle(), "Spring 2026");
}

TEST_F(TermRepositoryTest, UpdateStartDate) {
    Term term{validTitle, validStart, validEnd, false};
    repo_->insert(term);

    std::chrono::year_month_day newStart{2025y/9/1};
    term.setStartDate(newStart);
    repo_->update(term);

    auto result = repo_->findById(term.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getStartDate(), newStart);
}

TEST_F(TermRepositoryTest, UpdateEndDate) {
    Term term{validTitle, validStart, validEnd, false};
    repo_->insert(term);

    std::chrono::year_month_day newEnd{2025y/12/20};
    term.setEndDate(newEnd);
    repo_->update(term);

    auto result = repo_->findById(term.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getEndDate(), newEnd);
}

TEST_F(TermRepositoryTest, UpdateActive) {
    Term term{validTitle, validStart, validEnd, false};
    repo_->insert(term);

    term.setActive(true);
    repo_->update(term);

    auto result = repo_->findById(term.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_TRUE(result->getActive());
}

TEST_F(TermRepositoryTest, RemoveTerm) {
    Term term{validTitle, validStart, validEnd, false};
    repo_->insert(term);

    repo_->remove(term.getId());

    auto result = repo_->findById(term.getId());

    ASSERT_FALSE(result.has_value());
}

TEST_F(TermRepositoryTest, FindByIdNotFound) {
    auto result = repo_->findById("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee");

    ASSERT_FALSE(result.has_value());
}

TEST_F(TermRepositoryTest, FindAllEmpty) {
    auto results = repo_->findAll();

    // no terms inserted in this transaction; table should be empty
    ASSERT_EQ(results.size(), 0u);
}

TEST_F(TermRepositoryTest, FindByParentIdThrows) {
    ASSERT_THROW(repo_->findByParentId("any-id"), std::logic_error);
}

// ====================================
// EDGE CASES
// ====================================

TEST_F(TermRepositoryTest, InsertDuplicateTitleThrows) {
    Term term1{validTitle, validStart, validEnd, false};
    Term term2{validTitle,
        std::chrono::year_month_day{2026y/1/20},
        std::chrono::year_month_day{2026y/5/23},
        true};

    repo_->insert(term1);

    // duplicate title violates UNIQUE constraint on terms.title
    ASSERT_THROW(repo_->insert(term2), std::exception);
}

TEST_F(TermRepositoryTest, InsertRoundTripsId) {
    Term term{validTitle, validStart, validEnd, false};
    repo_->insert(term);

    auto result = repo_->findById(term.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getId(), term.getId());
}

TEST_F(TermRepositoryTest, InsertYearBoundaryDates) {
    Term term{validTitle,
        std::chrono::year_month_day{2024y/12/31},
        std::chrono::year_month_day{2025y/1/1},
        false};
    repo_->insert(term);

    auto result = repo_->findById(term.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getStartDate(), (std::chrono::year_month_day{2024y/12/31}));
    ASSERT_EQ(result->getEndDate(),   (std::chrono::year_month_day{2025y/1/1}));
}

TEST_F(TermRepositoryTest, InsertLeapYearDate) {
    Term term{validTitle,
        std::chrono::year_month_day{2024y/2/29},
        std::chrono::year_month_day{2024y/6/1},
        false};
    repo_->insert(term);

    auto result = repo_->findById(term.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getStartDate(), (std::chrono::year_month_day{2024y/2/29}));
}

TEST_F(TermRepositoryTest, InsertSameDayStartAndEnd) {
    Term term{validTitle,
        std::chrono::year_month_day{2025y/6/15},
        std::chrono::year_month_day{2025y/6/15},
        false};
    repo_->insert(term);

    auto result = repo_->findById(term.getId());

    ASSERT_TRUE(result.has_value());
    ASSERT_EQ(result->getStartDate(), result->getEndDate());
}

TEST_F(TermRepositoryTest, RemoveNonExistentIdNoThrow) {
    // removing a nonexistent id should not throw
    ASSERT_NO_THROW(repo_->remove("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee"));
}

TEST_F(TermRepositoryTest, FindAllReturnsTwoInsertedTerms) {
    Term term1{"Fall 2025", validStart, validEnd, false};
    Term term2{"Spring 2026",
        std::chrono::year_month_day{2026y/1/20},
        std::chrono::year_month_day{2026y/5/23},
        true};

    repo_->insert(term1);
    repo_->insert(term2);

    auto results = repo_->findAll();

    ASSERT_EQ(results.size(), 2u);
}