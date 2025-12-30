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
        Term &term;

    public:
        explicit Controller(Term &term);
        
        void addCourse(const std::string &courseName, int credits);
        void addAssignment(const std::string &courseId, const std::string &assignmentName, float grade);

        float getTermGpa() const;
        const std::unordered_map<std::string, Course>& getCourses() const;
};

#endif