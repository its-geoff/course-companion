#include <gtest/gtest.h>
#include <sstream>      // diverts output from terminal to separate stream
#include <regex>        // regular expression matching for UUIDs
#include "models/Term.hpp"
#include "app/utils.hpp"

using namespace std::chrono_literals;

// test fixture for class Term
class TermTest : public testing::Test {
    protected:
        Term term1{
            "Fall 2025", 
            std::chrono::year_month_day{2025y/8/12},
            std::chrono::year_month_day{2025y/12/5},
            true
        };
};

// ====================================
// GETTER SMOKE TESTS
// ====================================



// ====================================
// SETTER SMOKE TESTS
// ====================================



// ====================================
// INITIALIZATION SMOKE TESTS
// ====================================



// ====================================
// GETTER EDGE CASES
// ====================================



// ====================================
// SETTER EDGE CASES
// ====================================



// ====================================
// INITIALIZATION EDGE CASES
// ====================================



// ====================================
// CLASS USE CASES
// ====================================