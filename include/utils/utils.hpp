#ifndef UTILS_HPP
#define UTILS_HPP

/**
 * @file utils.hpp
 * @brief Definition of utilities that can be commonly used among all classes. 
 * 
 * This file defines functions that are independent of any class. These functions perform common
 * string manipulation, parsing, validation, or formatting.
 */

#include <string>           // for string variables
#include <chrono>           // for date and time-related variables
#include <iostream>         // for i/o streams
#include "model/Assignment.hpp"   // for references to Assignment

namespace utils {
    std::chrono::year_month_day getTodayDate();
    bool isOnlyWhitespace(const std::string str);
    std::string generateUuid();
    void validateTitle(std::string title);
    void validateDate(std::chrono::year_month_day dueDate);
    void validateDateOrder(std::chrono::year_month_day earlierDate, std::chrono::year_month_day laterDate);
    std::string boolToString(bool value);
    bool floatEqual(float a, float b, float relEps = std::numeric_limits<float>::epsilon() * 10, float absEps = 1e-8f);
    float floatRound(float value, int decimalPlaces);
    std::chrono::year_month_day defaultStartDate();
    std::chrono::year_month_day defaultEndDate(std::chrono::year_month_day startDate);
    std::chrono::year_month_day defaultEndDate(std::chrono::year_month_day startDate, int numMonths);
    std::string stringLower(std::string input);
    std::string stringTrim(const std::string str);

    // included in header since it's a template function
    // prints a map in the format "first -> second"
    template <typename T>
    void printMap(const T& map, std::ostream &os) {
        for (const auto& [first, second] : map) {
            os << first << " -> " << second << "\n";
        }
    }
}

#endif  // UTILS_HPP