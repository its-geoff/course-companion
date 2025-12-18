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
    bool isOnlyWhitespace(const std::string_view str);
    void validateTitle(std::string_view title);
    void validateDate(std::chrono::year_month_day dueDate);
    std::optional<std::string> readOptionalString(std::istream &is);
    std::optional<std::chrono::year_month_day> readOptionalDate(std::istream &is);
    std::optional<bool> readOptionalBool(std::istream &is);
    std::optional<float> readOptionalFloat(std::istream &is);
    Assignment chooseAssignmentConstructor(std::string title, std::optional<std::string> description, std::optional<std::chrono::year_month_day> date, 
        std::optional<bool> completed, std::optional<float> grade);
}

#endif