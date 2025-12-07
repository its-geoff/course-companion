#ifndef CONTROLLER_H
#define CONTROLLER_H

/**
 * @file Controller.hpp
 * 
 * @brief Definition of a controller that manages interaction between Assignment, Course, and Term. 
 * 
 * This controller defines functions that integrate the three classes together and allow for code 
 * reusability within the main function.
 */

#include <string>           // for string variables
#include <chrono>           // for date and time-related variables
#include <optional>         // for optional variables
#include "models/Assignment.hpp"   // for references to Assignment
// #include "Course.hpp"   // for connection to relevant Course  -> no Course implementation yet

/**
 * @class Controller
 * @brief Controls the interaction between Assignment, Course, and Term classes.
 * 
 * Manages the program’s workflow by coordinating interactions between the user interface and the data models. 
 * It stores the current application state, processes user input, and delegates operations to classes such as 
 * Term, Course, and Assignment.
 */
class Controller {
    private:
        // add after implementation of Term is completed
    public:
        Assignment chooseAssignmentConstructor(std::string title, std::optional<std::string> description, std::optional<std::chrono::year_month_day> date, 
            std::optional<bool> completed, std::optional<float> grade);
};

#endif