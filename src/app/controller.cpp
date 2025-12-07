#include "app/Controller.hpp"

/**
 * @file Controller.cpp
 * @brief Implementation of an a controller that manages interaction between Assignment, Course, and Term. 
 * 
 * This controller implements functions that integrate the three classes together and allow for code 
 * reusability within the main function.
 */

// chooses a constructor for the Assignment class based on the user input given
Assignment chooseAssignmentConstructor(std::string title, std::optional<std::string> description, std::optional<std::chrono::year_month_day> dueDate, 
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