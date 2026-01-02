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
#include <cmath>            // for fabs, min, and max
#include <limits>           // for numeric limits
#include <uuid/uuid.h>      // for UUID

namespace utils {
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

    // throws an exception if a string is empty
    void validateTitle(std::string title) {
        if (isOnlyWhitespace(title))
            throw std::invalid_argument("Title must be non-empty.");
    }

    // throws an exception if a date is non-existent
    void validateDate(std::chrono::year_month_day date) {
        if (!date.ok())
            throw std::invalid_argument("Date is invalid.");
    }

    // reads optional string from user input
    std::optional<std::string> readOptionalString(std::istream &is) {
        std::string response;
        std::getline(is, response);

        if (isOnlyWhitespace(response))
            return std::nullopt;

        return response;
    }

    // reads optional date from user input and converts to the year_month_day format
    std::optional<std::chrono::year_month_day> readOptionalDate(std::istream &is) {
        std::string response;
        std::getline(is, response);
        if (isOnlyWhitespace(response))
            return std::nullopt;

        int y;
        unsigned int m, d;
        if (sscanf(response.c_str(), "%d-%d-%d", &y, &m, &d) != 3)
            return std::nullopt;

        std::chrono::year_month_day output{std::chrono::year{y}, std::chrono::month{m}, std::chrono::day{d}};
        if (!output.ok())
            return std::nullopt;

        return output;
    }

    // reads optional bool from user input and converts to the bool format
    std::optional<bool> readOptionalBool(std::istream &is) {
        std::string response;
        std::getline(is, response);
        if (isOnlyWhitespace(response))
            return std::nullopt;

        if (response == "yes" || response == "y" || response == "true" || response == "1")
            return true;
        else if (response == "no" || response == "n" || response == "false" || response == "0")
            return false;

        return std::nullopt;
    }

    // reads optional float from user input and converts to the float format
    std::optional<float> readOptionalFloat(std::istream &is) {
        std::string response;
        std::getline(is, response);
        if (isOnlyWhitespace(response))
            return std::nullopt;

        return std::stof(response);
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

    std::chrono::year_month_day defaultStartDate() {
        return std::chrono::floor<std::chrono::days>(
            std::chrono::system_clock::now()
        );
    }

    // TO-DO: allow the user to change the default term length
    std::chrono::year_month_day defaultEndDate(std::chrono::year_month_day startDate) {
        return startDate + std::chrono::months{4};
    }
}