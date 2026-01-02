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

TEST(UtilsTest, DefaultStartDate) {
    std::chrono::year_month_day result = utils::defaultStartDate();
    // NOTE: must change line below before each test; time in UTC
    std::chrono::year_month_day todayDate = std::chrono::year_month_day{2026y/1/2};
    ASSERT_EQ(result, todayDate);
}

TEST(UtilsTest, DefaultEndDate) {
    std::chrono::year_month_day todayDate = std::chrono::year_month_day{2026y/1/2};
    std::chrono::year_month_day defaultEnd = todayDate + std::chrono::months{4};
    std::chrono::year_month_day result = utils::defaultEndDate(todayDate);
    ASSERT_EQ(result, defaultEnd);
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