#include "controller.hpp"

/**
 * @file controller.cpp
 * @brief Implementation of an a controller that manages interaction between Assignment, Course, and Term. 
 * 
 * This controller implements functions that integrate the three classes together and allow for code 
 * reusability within the main function.
 */

#include <algorithm>       // for all_of
#include <cctype>          // for isspace

// checks if a string is only whitespace
bool isOnlyWhitespace(const std::string_view str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isspace(c); });
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

// chooses a constructor for this class based on the user input given
Assignment chooseConstructor(std::string title, std::optional<std::string> description, std::optional<std::chrono::year_month_day> dueDate, 
    std::optional<bool> completed, std::optional<float> grade) {
    if (description && dueDate && completed && grade)
        return Assignment(title, *description, *dueDate, *completed, *grade);
    else if (description && dueDate && completed)
        return Assignment(title, *description, *dueDate, *completed);
    else if (description && dueDate)
        return Assignment(title, *description, *dueDate);
    else if (description)
        return Assignment(title, *description);
    
    if (dueDate && completed && grade)
        return Assignment(title, *dueDate, *completed, *grade);
    else if (dueDate && completed)
        return Assignment(title, *dueDate, *completed);
    else if (dueDate)
        return Assignment(title, *dueDate);

    return Assignment(title);
}