#include <gtest/gtest.h>
#include <sstream>
#include <regex>        // for UUID testing
#include <unordered_set>    // for UUID testing
#include "utils/utils.hpp"

using namespace std::chrono_literals;

// ====================================
// FUNCTION SMOKE TESTS
// ====================================

TEST(UtilsTest, GenerateUuidNotEmpty) {
    std::string uuid = utils::generateUuid();
    // check if uuid is not empty
    ASSERT_FALSE(utils::isOnlyWhitespace(uuid));
}

TEST(UtilsTest, GenerateUuidFormat) {
    std::string uuid = utils::generateUuid();

    // allow both letter cases for cross-platform testing
    std::regex uuidRegex(
        "^[0-9a-fA-F]{8}-"
        "[0-9a-fA-F]{4}-"
        "[0-9a-fA-F]{4}-"
        "[0-9a-fA-F]{4}-"
        "[0-9a-fA-F]{12}$"
    );

    ASSERT_TRUE(std::regex_match(uuid, uuidRegex));
}

TEST(UtilsTest, GenerateUuidLength) {
    std::string uuid = utils::generateUuid();
    ASSERT_EQ(uuid.length(), 36);
}

TEST(UtilsTest, ReadOptionalString) {
    std::stringstream ss("Homework 2");
    ASSERT_EQ(utils::readOptionalString(ss).value(), "Homework 2");
}

TEST(UtilsTest, ReadOptionalDate) {
    std::stringstream ss("2025-10-31");
    ASSERT_EQ(utils::readOptionalDate(ss).value(), std::chrono::year_month_day{2025y/10/31});
}

TEST(UtilsTest, ReadOptionalBool) {
    std::stringstream ss("yes");
    ASSERT_EQ(utils::readOptionalBool(ss).value(), true);
}

TEST(UtilsTest, ReadOptionalFloat) {
    std::stringstream ss("95.18");
    ASSERT_FLOAT_EQ(utils::readOptionalFloat(ss).value(), 95.18);
}

TEST(UtilsTest, BoolToString) {
    Assignment assignment1{"Homework 3", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};
    Assignment assignment2{"Homework 1", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};
    ASSERT_EQ(utils::boolToString(assignment1.getCompleted()), "Yes");
    ASSERT_EQ(utils::boolToString(assignment2.getCompleted()), "No");
}

TEST(UtilsTest, FloatEqual) {
    ASSERT_TRUE(utils::floatEqual(1.0f, 1.0f));
    ASSERT_FALSE(utils::floatEqual(1.0f, 1.1f));
}

TEST(UtilsTest, PrintMap) {
    std::map<int, std::string> numbers = {
        {1, "one"},
        {2, "two"},
        {3, "three"}
    };

    std::stringstream ss;
    utils::printMap(numbers, ss);

    ASSERT_EQ(ss.str(), ("1 -> one\n2 -> two\n3 -> three\n"));
}

TEST(UtilsTest, ChooseAssignmentConstructor) {
    Assignment assignment2 = utils::chooseAssignmentConstructor(
        "Homework 1", 
        "Focus on lexical analysis.", 
        std::chrono::year_month_day{2025y/10/31}, 
        false, 
        90.50f
    );

    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
    ASSERT_FALSE(assignment2.getCompleted());
    ASSERT_FLOAT_EQ(assignment2.getGrade(), 90.50f);
}

// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST(UtilsTest, UuidUniqueness) {
    std::unordered_set<std::string> uuids;

    int n = 1000;
    for (int i = 0; i < n; ++i) {
        std::string uuid = utils::generateUuid();
        auto result = uuids.insert(uuid);
        ASSERT_TRUE(result.second);
    }

    // to check uniqueness, compare size of unordered set to number of UUIDs
    // (unordered set doesn't include duplicates)
    ASSERT_EQ(uuids.size(), static_cast<std::size_t>(n));
}

TEST(UtilsTest, ReadOptionalStringEmpty) {
    std::stringstream ss1("");
    std::stringstream ss2("  ");

    ASSERT_EQ(utils::readOptionalString(ss1), std::nullopt);
    ASSERT_EQ(utils::readOptionalString(ss2), std::nullopt);
}

TEST(UtilsTest, ReadOptionalDateEmpty) {
    std::stringstream ss1("");
    std::stringstream ss2("  ");

    ASSERT_EQ(utils::readOptionalDate(ss1), std::nullopt);
    ASSERT_EQ(utils::readOptionalDate(ss2), std::nullopt);
}

TEST(UtilsTest, ReadOptionalDateMissingValue) {
    std::stringstream ss1("2025-02-");

    ASSERT_EQ(utils::readOptionalDate(ss1), std::nullopt);
}

TEST(UtilsTest, ReadOptionalDateInvalid) {
    std::stringstream ss1("2025-02-30");

    ASSERT_EQ(utils::readOptionalDate(ss1), std::nullopt);
}

TEST(UtilsTest, ReadOptionalBoolEmpty) {
    std::stringstream ss1("");
    std::stringstream ss2("  ");

    ASSERT_EQ(utils::readOptionalBool(ss1), std::nullopt);
    ASSERT_EQ(utils::readOptionalBool(ss2), std::nullopt);
}

TEST(UtilsTest, ReadOptionalBoolInvalid) {
    std::stringstream ss1("ye");

    ASSERT_EQ(utils::readOptionalBool(ss1), std::nullopt);
}

TEST(UtilsTest, ReadOptionalBoolAlternatives) {
    std::stringstream ss_y(
        "yes\n"
        "y\n"
        "true\n"
        "1\n"
    );
    std::stringstream ss_n(
        "no\n"
        "n\n"
        "false\n"
        "0\n"
    );

    // checks each line separately and asserts true or false, based on conversion
    ASSERT_EQ(utils::readOptionalBool(ss_y).value(), true);
    ASSERT_EQ(utils::readOptionalBool(ss_y).value(), true);
    ASSERT_EQ(utils::readOptionalBool(ss_y).value(), true);
    ASSERT_EQ(utils::readOptionalBool(ss_y).value(), true);
    ASSERT_EQ(utils::readOptionalBool(ss_n).value(), false);
    ASSERT_EQ(utils::readOptionalBool(ss_n).value(), false);
    ASSERT_EQ(utils::readOptionalBool(ss_n).value(), false);
    ASSERT_EQ(utils::readOptionalBool(ss_n).value(), false);
}

TEST(UtilsTest, ReadOptionalFloatEmpty) {
    std::stringstream ss1("");
    std::stringstream ss2("  ");

    ASSERT_EQ(utils::readOptionalFloat(ss1), std::nullopt);
    ASSERT_EQ(utils::readOptionalFloat(ss2), std::nullopt);
}

TEST(UtilsTest, FloatEqualVerySmall) {
    ASSERT_TRUE(utils::floatEqual(0.0f, 1e-9f));
}

TEST(UtilsTest, FloatEqualVeryLarge) {
    ASSERT_TRUE(utils::floatEqual(1e8f, 1e8f + 1.0f));
}

TEST(UtilsTest, FloatEqualNanValue) {
    ASSERT_FALSE(utils::floatEqual(1.0f, std::nanf("")));
}

TEST(UtilsTest, ChooseAssignmentConstructorTitleOnly) {
    Assignment assignment2 = utils::chooseAssignmentConstructor(
        "Homework 1", 
        std::nullopt,
        std::nullopt,
        std::nullopt,
        std::nullopt
    );

    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "");
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::time_point_cast<std::chrono::days>(now);    // set to today
    ASSERT_EQ(assignment2.getDueDate(), today);
    ASSERT_FALSE(assignment2.getCompleted());
    ASSERT_FLOAT_EQ(assignment2.getGrade(), 0.0f);
}

TEST(UtilsTest, ChooseAssignmentConstructorTitleDueDate) {
    Assignment assignment2 = utils::chooseAssignmentConstructor(
        "Homework 1", 
        std::nullopt,
        std::chrono::year_month_day{2025y/10/31},
        std::nullopt,
        std::nullopt
    );

    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
    ASSERT_FALSE(assignment2.getCompleted());
    ASSERT_FLOAT_EQ(assignment2.getGrade(), 0.0f);
}

TEST(UtilsTest, ChooseAssignmentConstructorTitleDueDateCompleted) {
    Assignment assignment2 = utils::chooseAssignmentConstructor(
        "Homework 1", 
        std::nullopt,
        std::chrono::year_month_day{2025y/10/31},
        true,
        std::nullopt
    );

    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
    ASSERT_TRUE(assignment2.getCompleted());
    ASSERT_FLOAT_EQ(assignment2.getGrade(), 0.0f);
}

TEST(UtilsTest, ChooseAssignmentConstructorTitleDueDateCompletedGrade) {
    Assignment assignment2 = utils::chooseAssignmentConstructor(
        "Homework 1", 
        std::nullopt,
        std::chrono::year_month_day{2025y/10/31},
        true,
        98.15f
    );

    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
    ASSERT_TRUE(assignment2.getCompleted());
    ASSERT_FLOAT_EQ(assignment2.getGrade(), 98.15f);
}

TEST(UtilsTest, ChooseAssignmentConstructorTitleDesc) {
    Assignment assignment2 = utils::chooseAssignmentConstructor(
        "Homework 1", 
        "Focus on lexical analysis.",
        std::nullopt,
        std::nullopt,
        std::nullopt
    );

    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::time_point_cast<std::chrono::days>(now);    // set to today
    ASSERT_EQ(assignment2.getDueDate(), today);
    ASSERT_FALSE(assignment2.getCompleted());
    ASSERT_FLOAT_EQ(assignment2.getGrade(), 0.0f);
}

TEST(UtilsTest, ChooseAssignmentConstructorTitleDescDueDate) {
    Assignment assignment2 = utils::chooseAssignmentConstructor(
        "Homework 1", 
        "Focus on lexical analysis.",
        std::chrono::year_month_day{2025y/10/31},
        std::nullopt,
        std::nullopt
    );

    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
    ASSERT_FALSE(assignment2.getCompleted());
    ASSERT_FLOAT_EQ(assignment2.getGrade(), 0.0f);
}

TEST(UtilsTest, ChooseAssignmentConstructorTitleDescDueDateCompleted) {
    Assignment assignment2 = utils::chooseAssignmentConstructor(
        "Homework 1", 
        "Focus on lexical analysis.",
        std::chrono::year_month_day{2025y/10/31},
        true,
        std::nullopt
    );

    ASSERT_EQ(assignment2.getTitle(), "Homework 1");
    ASSERT_EQ(assignment2.getDescription(), "Focus on lexical analysis.");
    ASSERT_EQ(assignment2.getDueDate(), std::chrono::year_month_day{2025y/10/31});
    ASSERT_TRUE(assignment2.getCompleted());
    ASSERT_FLOAT_EQ(assignment2.getGrade(), 0.0f);
}