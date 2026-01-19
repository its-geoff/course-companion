#include "utils/utils.hpp"

/**
 * @file utils.cpp
 * @brief Implementation of utilities that can be used across multiple classes.
 * 
 * These utility functions are independent of any class. They perform common operations like
 * string manipulation, parsing and validation. These functions are typically needed by
 * multiple components.
 */

#include <algorithm>        // for all_of
#include <cctype>           // for isspace
#include <cmath>            // for fabs, min, max, and round
#include <limits>           // for numeric limits
#include <uuid/uuid.h>      // for UUID

using namespace std::chrono_literals;

namespace utils {
    // get today's date to use for tests
    std::chrono::year_month_day getTodayDate() {
        return std::chrono::year_month_day{floor<std::chrono::days>(std::chrono::system_clock::now())};
    } 

    // checks if a string is only whitespace
    bool isOnlyWhitespace(const std::string str) {
        return std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isspace(c); });
    }

    // generates a unique ID across all objects and classes
    std::string generateUuid() {
        uuid_t id;
        uuid_generate_random(id);   // use UUID v4 for random generation
        char str[37];
        uuid_unparse(id, str);
        return std::string(str);
    }

    // throws an exception if a required string is empty
    void validateReqString(std::string str, std::string label) {
        if (isOnlyWhitespace(str))
            throw std::invalid_argument(label + " must be non-empty.");
    }

    // throws an exception if a date is non-existent
    void validateDate(std::chrono::year_month_day date) {
        if (!date.ok())
            throw std::invalid_argument("Date is invalid.");
    }

    // throws an exception if end date is before start date
    void validateDateOrder(std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) {
        if (endDate < startDate) {
            throw std::logic_error("End date cannot be before start date.");
        }
    } 

    // converts bool value of completed into a string for output
    std::string boolToString(bool value) {
        if (value)
            return "Yes";
        else
            return "No";
    }

    // checks if two floats are equal while taking into account relative and absolute tolerance
    bool floatEqual(float a, float b, float relEps, float absEps) {
        if (a == b)
            return true;

        if (std::isnan(a) || std::isnan(b))
            return false;

        return std::fabs(a - b) <= std::max(relEps * std::max(std::fabs(a), std::fabs(b)), absEps);
    }

    // rounds a float to a specified number of decimal places
    float floatRound(float value, int decimalPlaces) {
        // scale number, round to int, then unscale
        float scaledNum = std::pow(10.0f, decimalPlaces);
        return std::round(value * scaledNum) / scaledNum;
    }

    // returns today's date as the default start date
    std::chrono::year_month_day defaultStartDate() {
        auto today = std::chrono::floor<std::chrono::days>(
            std::chrono::system_clock::now()
        );
        return std::chrono::year_month_day{today};
    }

    // returns start date + 4 months as the default end date
    std::chrono::year_month_day defaultEndDate(std::chrono::year_month_day startDate) {
        return startDate + std::chrono::months{4};
    }

    // TO-DO: add this to settings when that feature is added
    // returns start date + user-specified interval as the default end date
    std::chrono::year_month_day defaultEndDate(std::chrono::year_month_day startDate, int numMonths) {
        if (numMonths < 1) {
            // ensures that default end is not the same as default start
            throw std::out_of_range("Number of months must be 1 or greater.");
        }

        return startDate + std::chrono::months{numMonths};
    }

    // transform string to all lowercase
    std::string stringLower(std::string input) {
        std::transform(input.begin(), input.end(), input.begin(),
            [](unsigned char c){ return std::tolower(c); });

        return input;
    }

    // trims leading and trailing whitespace from a string
    std::string stringTrim(const std::string str) {
        auto start = std::find_if_not(str.begin(), str.end(),
            [](unsigned char c) { return std::isspace(c); });

        auto end = std::find_if_not(str.rbegin(), str.rend(),
            [](unsigned char c) { return std::isspace(c); }).base();

        if (start >= end) {
            return "";
        }

        return std::string(start, end);
    }
}