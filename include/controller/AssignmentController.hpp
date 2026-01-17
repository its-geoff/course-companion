#ifndef ASSIGNMENTCONTROLLER_HPP
#define ASSIGNMENTCONTROLLER_HPP

/**
 * @file AssignmentController.hpp
 * @brief Definition of a controller that manages interaction between Assignment and the views.
 * 
 * This controller defines functions that integrate the Assignment class with the views and allow for the 
 * model to connect to the main function.
 */

#include <string>           // for string variables
#include <chrono>           // for date and time-related variables
#include <unordered_map>    // for Assignment objects
#include "model/Course.hpp"     // for Course reference
#include "model/Assignment.hpp"

/**
 * @class AssignmentController
 * @brief Controls the interaction between the Assignment class and the views.
 * 
 * Manages the program’s workflow by coordinating interactions between the user interface and the Assignment class. 
 * It stores the current application state and processes user input. This class completes delegated operations 
 * from the views and provides output to the user through the main function.
 */
class AssignmentController {
    private:
        Course& course_;
        Assignment* activeAssignment_ = nullptr;
        std::unordered_map<std::string, std::string> titleToId_{};  // title -> id, titles in lowercase for easier comparison

    public:
        AssignmentController(Course& course);
        // prevent copies and require references
        AssignmentController(const AssignmentController&) = delete;
        AssignmentController& operator=(const AssignmentController&) = delete;
        // allows objects to move for instantiation; cannot move references after construction
        AssignmentController(AssignmentController&&) = default;
        AssignmentController& operator=(AssignmentController&&) = delete;

        const std::unordered_map<std::string, Assignment>& getAssignmentList() const;
        std::string getAssignmentId(const std::string& title) const;

        void addAssignment(const std::string& title, const std::string& description, 
            const std::chrono::year_month_day& dueDate, bool completed, float grade);
        void editTitle(const std::string& id, const std::string& newTitle);
        void editDescription(const std::string& id, const std::string& newDescription);
        void editDueDate(const std::string& id, const std::chrono::year_month_day& newDueDate);
        void editCompleted(const std::string& id, bool newCompleted);
        void editGrade(const std::string& id, float newGrade);
        void removeAssignment(const std::string& title);
        const Assignment& findAssignment(const std::string& title) const;   // non-mutable version
        Assignment &findAssignment(const std::string& title);   // mutable version
        void selectAssignment(const std::string& title);
};

#endif