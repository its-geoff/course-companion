#ifndef CONTROLLER_H
#define CONTROLLER_H

/**
 * @file controller.hpp
 * 
 * @brief Definition of an a controller that manages interaction between Assignment, Course, and Term. 
 * 
 * This controller defines functions that integrate the three classes together and allow for code 
 * reusability within the main function.
 */

#include <string>           // for string variables
#include <chrono>           // for date and time-related variables
#include <optional>         // for optional variables
#include <iostream>         // for i/o streams
#include "../models/Assignment.hpp"   // for references to Assignment
// #include "Course.hpp"   // for connection to relevant Course  -> no Course implementation yet

bool isOnlyWhitespace(const std::string_view str);
std::optional<std::string> readOptionalString(std::istream &is);
std::optional<std::chrono::year_month_day> readOptionalDate(std::istream &is);
std::optional<bool> readOptionalBool(std::istream &is);
std::optional<float> readOptionalFloat(std::istream &is);
Assignment chooseAssignmentConstructor(std::string title, std::optional<std::string> description, std::optional<std::chrono::year_month_day> date, 
    std::optional<bool> completed, std::optional<float> grade);
// next, implement addAssignment and removeAssignment
void addAssignment();
void removeAssignment();

#endif