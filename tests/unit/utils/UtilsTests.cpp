#include <gtest/gtest.h>
#include <sstream>
#include <regex>        // for UUID testing
#include <unordered_set>    // for UUID testing
#include <limits>       // for numeric_limits
#include <exception>    // for exception throwing
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

TEST(UtilsTest, BoolToString) {
    Assignment assignment1{"Homework 3", "", std::chrono::year_month_day{2025y/11/20}, true, 95.18f};
    Assignment assignment2{"Homework 1", "", std::chrono::year_month_day{2025y/10/31}, false, 90.50f};
    ASSERT_EQ(utils::boolToString(assignment1.getCompleted()), "Yes");
    ASSERT_EQ(utils::boolToString(assignment2.getCompleted()), "No");
}

TEST(UtilsTest, FloatEqual) {
    ASSERT_TRUE(utils::floatEqual(1.0f, 1.0f));
    ASSERT_FALSE(utils::floatEqual(1.0f, 1.1f));
}

TEST(UtilsTest, FloatRound) {
    ASSERT_FLOAT_EQ(utils::floatRound(48.271905f, 2), 48.27f);
    ASSERT_FLOAT_EQ(utils::floatRound(1.578918579f, 5), 1.57892f);
}

TEST(UtilsTest, DefaultStartDate) {
    std::chrono::year_month_day result = utils::defaultStartDate();
    std::chrono::year_month_day todayDate = utils::getTodayDate();
    ASSERT_EQ(result, todayDate);
}

TEST(UtilsTest, DefaultEndDate) {
    std::chrono::year_month_day todayDate = utils::getTodayDate();
    std::chrono::year_month_day defaultEnd = todayDate + std::chrono::months{4};
    std::chrono::year_month_day result = utils::defaultEndDate(todayDate);
    ASSERT_EQ(result, defaultEnd);
}

TEST(UtilsTest, DefaultEndDateUserInput) {
    std::chrono::year_month_day todayDate = utils::getTodayDate();
    std::chrono::year_month_day defaultEnd = todayDate + std::chrono::months{6};
    std::chrono::year_month_day result = utils::defaultEndDate(todayDate, 6);
    ASSERT_EQ(result, defaultEnd);
}

TEST(UtilsTest, StringLower) {
    std::string input{"HELLO WORLD"};
    ASSERT_EQ(utils::stringLower(input), "hello world");
}

TEST(UtilsTest, StringTrim) {
    std::string str{"  space  "};
    ASSERT_EQ(utils::stringTrim(str), "space");
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

TEST(UtilsTest, FloatEqualVerySmall) {
    ASSERT_TRUE(utils::floatEqual(0.0f, 1e-9f));
}

TEST(UtilsTest, FloatEqualVeryLarge) {
    ASSERT_TRUE(utils::floatEqual(1e8f, 1e8f + 1.0f));
}

TEST(UtilsTest, FloatEqualNanValue) {
    ASSERT_FALSE(utils::floatEqual(1.0f, std::numeric_limits<float>::quiet_NaN()));
}

TEST(UtilsTest, FloatRoundAlreadyRounded) {
    ASSERT_FLOAT_EQ(utils::floatRound(1.0000000f, 2), 1.00f);
    ASSERT_FLOAT_EQ(utils::floatRound(3.1400f, 3), 3.140f);
}

TEST(UtilsTest, DefaultEndDateUserInputInvalid) {
    std::chrono::year_month_day todayDate = utils::getTodayDate();
    ASSERT_THROW(utils::defaultEndDate(todayDate, -3), std::out_of_range);
}

TEST(UtilsTest, StringLowerMixedCase) {
    std::string input1{"tEsTiNg"};
    std::string input2{"TESTing"};
    std::string input3{"testING"};

    ASSERT_EQ(utils::stringLower(input1), "testing");
    ASSERT_EQ(utils::stringLower(input2), "testing");
    ASSERT_EQ(utils::stringLower(input3), "testing");
}

TEST(UtilsTest, StringTrimLeadingWhitespace) {
    std::string str{"      sock"};
    ASSERT_EQ(utils::stringTrim(str), "sock");
}

TEST(UtilsTest, StringTrimTrailingWhitespace) {
    std::string str{"weight          "};
    ASSERT_EQ(utils::stringTrim(str), "weight");
}

TEST(UtilsTest, StringTrimNoWhitespace) {
    std::string str{"hello"};
    ASSERT_EQ(utils::stringTrim(str), "hello");
}