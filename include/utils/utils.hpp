#ifndef UTILS_H
#define UTILS_H

/**
 * @file utils.hpp
 * 
 * @brief Definition of utilities that can be commonly used among all classes. 
 * 
 * This file defines functions that are independent of any class. These functions perform common
 * string manipulation, parsing, validation, or formatting.
 */

#include <string>           // for string variables
#include <chrono>           // for date and time-related variables
#include <optional>         // for optional variables
#include <iostream>         // for i/o streams
#include "models/Assignment.hpp"   // for references to Assignment

namespace utils {
    bool isOnlyWhitespace(const std::string str);
    std::string generateUuid();
    void validateTitle(std::string title);
    void validateDate(std::chrono::year_month_day dueDate);
    std::optional<std::string> readOptionalString(std::istream &is = std::cin);
    std::optional<std::chrono::year_month_day> readOptionalDate(std::istream &is = std::cin);
    std::optional<bool> readOptionalBool(std::istream &is = std::cin);
    std::optional<float> readOptionalFloat(std::istream &is = std::cin);
    std::string boolToString(bool value);
    bool floatEqual(float a, float b, float relEps = std::numeric_limits<float>::epsilon() * 10, float absEps = 1e-8f);

    // included in header since it's a template function
    // prints a map in the format "first -> second"
    template <typename T>
    void printMap(const T& map, std::ostream &os) {
        for (const auto& [first, second] : map) {
            os << first << " -> " << second << "\n";
        }
    }
}

#endif