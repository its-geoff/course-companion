#ifndef COURSECONTROLLER_HPP
#define COURSECONTROLLER_HPP

/**
 * @file CourseController.hpp
 * @brief Definition of a controller that manages interaction between Course and the views.
 * 
 * This controller defines functions that integrate the Course class with the views and allow for the 
 * model to connect to the main function.
 */

#include <string>           // for string variables
#include <chrono>           // for date and time-related variables
#include <unordered_map>    // for Course objects
#include <optional>         // for AssignmentController
#include "model/Term.hpp"   // for Term reference
#include "model/Course.hpp"

/**
 * @class CourseController
 * @brief Controls the interaction between the Course class and the views.
 * 
 * Manages the program’s workflow by coordinating interactions between the user interface and the Course class. 
 * It stores the current application state and processes user input. This class completes delegated operations 
 * from the views and provides output to the user through the main function.
 */
class CourseController {
    private:
        Term& term_;
        Course* activeCourse_ = nullptr;
        std::unordered_map<std::string, std::string> titleToId_{}; // title -> id, titles in lowercase for easier comparison
        // std::optional<AssignmentController> assignmentController_{};

    public:
        CourseController(Term& term);
        // prevent copies and require references
        CourseController(const CourseController&) = delete;
        CourseController& operator=(const CourseController&) = delete;
        // allows objects to move for instantiation; cannot move references after construction
        CourseController(CourseController&&) = default;
        CourseController& operator=(CourseController&&) = delete;

        const std::unordered_map<std::string, Course>& getCourseList() const;
        std::string getCourseId(const std::string &title) const;

        void addCourse(const std::string& title, const std::string& description, const std::chrono::year_month_day& startDate, 
            const std::chrono::year_month_day& endDate, int numCredits, bool active);
        void editTitle(const std::string& id, const std::string& newTitle);
        void editDescription(const std::string& id, const std::string& newDescription);
        void editStartDate(const std::string& id, const std::chrono::year_month_day& newStartDate);
        void editEndDate(const std::string& id, const std::chrono::year_month_day &newEndDate);
        void editNumCredits(const std::string& id, int newNumCredits);
        void editActive(const std::string& id, bool newActive);
        void removeCourse(const std::string& title);
        const Course& findCourse(const std::string& title) const;   // non-mutable version
        Course &findCourse(const std::string& title);   // mutable version
        void selectCourse(const std::string& title);
};

#endif