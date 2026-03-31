#include <gtest/gtest.h>
#include <chrono>
#include <string>
#include "model/Term.hpp"
#include "utils/utils.hpp"

using namespace std::chrono_literals;

// test fixture for class TermRepository
class TermRepositoryTest : public testing::Test {
    protected:
        const std::string validId    = "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee";
        const std::string validTitle = "Fall 2025";
        const std::chrono::year_month_day validStart{2025y/8/12};
        const std::chrono::year_month_day validEnd{2025y/12/5};
};

// ====================================
// SMOKE TESTS
// ====================================

TEST_F(TermRepositoryTest, FromRowAllFields) {
    Term term = Term::fromRow(validId, validTitle, validStart, validEnd, false);

    ASSERT_EQ(term.getId(),        validId);
    ASSERT_EQ(term.getTitle(),     validTitle);
    ASSERT_EQ(term.getStartDate(), validStart);
    ASSERT_EQ(term.getEndDate(),   validEnd);
    ASSERT_FALSE(term.getActive());
}

TEST_F(TermRepositoryTest, FromRowActiveFlagTrue) {
    Term term = Term::fromRow(validId, validTitle, validStart, validEnd, true);
    ASSERT_TRUE(term.getActive());
}

TEST_F(TermRepositoryTest, FromRowActiveFlagFalse) {
    Term term = Term::fromRow(validId, validTitle, validStart, validEnd, false);
    ASSERT_FALSE(term.getActive());
}

TEST_F(TermRepositoryTest, FromRowPreservesId) {
    std::string id = "ffffffff-ffff-ffff-ffff-ffffffffffff";
    Term term = Term::fromRow(id, validTitle, validStart, validEnd, false);
    ASSERT_EQ(term.getId(), id);
}

TEST_F(TermRepositoryTest, FromRowTwoTermsRetainSeparateIds) {
    std::string id1 = "550e8400-e29b-41d4-a716-446655440000";
    std::string id2 = "550e8400-e29b-41d4-a716-446655440001";

    Term term1 = Term::fromRow(id1, "Fall 2025", validStart, validEnd, false);
    Term term2 = Term::fromRow(id2, "Spring 2026",
        std::chrono::year_month_day{2026y/1/20},
        std::chrono::year_month_day{2026y/5/23},
        true);

    ASSERT_EQ(term1.getId(), id1);
    ASSERT_EQ(term2.getId(), id2);
}

TEST_F(TermRepositoryTest, FromRowDoesNotEqualNewTerm) {
    // a fromRow Term and a freshly constructed Term with the same parameters should not be equal
    Term fromRowTerm = Term::fromRow(validId, validTitle, validStart, validEnd, false);
    Term newTerm{validTitle, validStart, validEnd, false};

    ASSERT_NE(fromRowTerm, newTerm);
}

// ====================================
// EDGE CASES
// ====================================

TEST_F(TermRepositoryTest, FromRowYearBoundaryDates) {
    Term term = Term::fromRow(validId, validTitle,
        std::chrono::year_month_day{2024y/12/31},
        std::chrono::year_month_day{2025y/1/1},
        false);

    ASSERT_EQ(term.getStartDate(), (std::chrono::year_month_day{2024y/12/31}));
    ASSERT_EQ(term.getEndDate(),   (std::chrono::year_month_day{2025y/1/1}));
}

TEST_F(TermRepositoryTest, FromRowLeapYearDate) {
    Term term = Term::fromRow(validId, validTitle,
        std::chrono::year_month_day{2024y/2/29},
        std::chrono::year_month_day{2024y/6/1},
        false);

    ASSERT_EQ(term.getStartDate(), (std::chrono::year_month_day{2024y/2/29}));
}

TEST_F(TermRepositoryTest, FromRowSameDayStartAndEnd) {
    Term term = Term::fromRow(validId, validTitle,
        std::chrono::year_month_day{2025y/6/15},
        std::chrono::year_month_day{2025y/6/15},
        false);

    ASSERT_EQ(term.getStartDate(), term.getEndDate());
}

TEST_F(TermRepositoryTest, FromRowEmptyTitle) {
    // throw invalid argument since title is empty
    ASSERT_THROW(Term::fromRow(validId, "",
        validStart, validEnd, false), std::invalid_argument);
}

TEST_F(TermRepositoryTest, FromRowWhitespaceTitle) {
    // throw invalid argument since title is whitespace
    ASSERT_THROW(Term::fromRow(validId, "   ",
        validStart, validEnd, false), std::invalid_argument);
}

TEST_F(TermRepositoryTest, FromRowInvalidStartDate) {
    // throw invalid argument since start date does not exist
    ASSERT_THROW(Term::fromRow(validId, validTitle,
        std::chrono::year_month_day{2025y/2/30},
        validEnd,
        false), std::invalid_argument);
}

TEST_F(TermRepositoryTest, FromRowInvalidEndDate) {
    // throw invalid argument since end date does not exist
    ASSERT_THROW(Term::fromRow(validId, validTitle,
        validStart,
        std::chrono::year_month_day{2025y/4/31},
        false), std::invalid_argument);
}

TEST_F(TermRepositoryTest, FromRowEndDateBeforeStartDate) {
    // throw logic error since end date before start date
    ASSERT_THROW(Term::fromRow(validId, validTitle,
        std::chrono::year_month_day{2025y/12/5},
        std::chrono::year_month_day{2025y/8/12},
        false), std::logic_error);
}

TEST_F(TermRepositoryTest, FromRowEmptyDates) {
    // throw invalid argument since dates are empty
    ASSERT_THROW(Term::fromRow(validId, validTitle,
        std::chrono::year_month_day{},
        std::chrono::year_month_day{},
        false), std::invalid_argument);
}