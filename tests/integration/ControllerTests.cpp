#include <gtest/gtest.h>
#include <sstream>
#include "app/controller.hpp"

using namespace std::chrono_literals;

// ====================================
// FUNCTION SMOKE TESTS
// ====================================

TEST(ControllerTest, ReadOptionalString) {
    std::stringstream ss("Homework 2");
    ASSERT_EQ(readOptionalString(ss).value(), "Homework 2");
}

TEST(ControllerTest, ReadOptionalDate) {
    std::stringstream ss("2025-10-31");
    ASSERT_EQ(readOptionalDate(ss).value(), std::chrono::year_month_day{2025y/10/31});
}

TEST(ControllerTest, ReadOptionalBool) {
    std::stringstream ss("yes");
    ASSERT_EQ(readOptionalBool(ss).value(), true);
}

TEST(ControllerTest, ReadOptionalFloat) {
    std::stringstream ss("95.18");
    ASSERT_FLOAT_EQ(readOptionalFloat(ss).value(), 95.18);
}

// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST(ControllerTest, ReadOptionalStringEmpty) {
    std::stringstream ss1("");
    std::stringstream ss2("  ");

    ASSERT_EQ(readOptionalString(ss1), std::nullopt);
    ASSERT_EQ(readOptionalString(ss2), std::nullopt);
}

TEST(ControllerTest, ReadOptionalDateEmpty) {
    std::stringstream ss1("");
    std::stringstream ss2("  ");

    ASSERT_EQ(readOptionalDate(ss1), std::nullopt);
    ASSERT_EQ(readOptionalDate(ss2), std::nullopt);
}

TEST(ControllerTest, ReadOptionalBoolEmpty) {
    std::stringstream ss1("");
    std::stringstream ss2("  ");

    ASSERT_EQ(readOptionalBool(ss1), std::nullopt);
    ASSERT_EQ(readOptionalBool(ss2), std::nullopt);
}

TEST(ControllerTest, ReadOptionalFloatEmpty) {
    std::stringstream ss1("");
    std::stringstream ss2("  ");

    ASSERT_EQ(readOptionalFloat(ss1), std::nullopt);
    ASSERT_EQ(readOptionalFloat(ss2), std::nullopt);
}

TEST(ControllerTest, ReadOptionalBoolAlternatives) {
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
    ASSERT_EQ(readOptionalBool(ss_y).value(), true);
    ASSERT_EQ(readOptionalBool(ss_y).value(), true);
    ASSERT_EQ(readOptionalBool(ss_y).value(), true);
    ASSERT_EQ(readOptionalBool(ss_y).value(), true);
    ASSERT_EQ(readOptionalBool(ss_n).value(), false);
    ASSERT_EQ(readOptionalBool(ss_n).value(), false);
    ASSERT_EQ(readOptionalBool(ss_n).value(), false);
    ASSERT_EQ(readOptionalBool(ss_n).value(), false);
}

// ====================================
// FUNCTION USE CASES
// ====================================