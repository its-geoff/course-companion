#include <gtest/gtest.h>
#include <sstream>
#include "controller.hpp"

using namespace std::chrono_literals;

// ====================================
// FUNCTION SMOKE TESTS
// ====================================

TEST(ControllerTest, ReadOptionalString) {
    std::stringstream ss("Homework 2");
    ASSERT_EQ(assignment1.readOptionalString(ss).value(), "Homework 2");
}

TEST(ControllerTest, ReadOptionalDate) {
    std::stringstream ss("2025-10-31");
    ASSERT_EQ(assignment1.readOptionalDate(ss).value(), std::chrono::year_month_day{2025y/10/31});
}

TEST(ControllerTest, ReadOptionalBool) {
    std::stringstream ss("yes");
    ASSERT_EQ(assignment1.readOptionalBool(ss).value(), true);
}

TEST(ControllerTest, ReadOptionalFloat) {
    std::stringstream ss("95.18");
    ASSERT_FLOAT_EQ(assignment1.readOptionalFloat(ss).value(), 95.18);
}

// ====================================
// FUNCTION EDGE CASES
// ====================================


// ====================================
// FUNCTION USE CASES
// ====================================