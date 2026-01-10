#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

/**
 * @file Assignment.hpp
 * @brief Definition of the Assignment class, which stores metadata and is used to organize coursework. 
 * 
 * There can be multiple Assignments within one Course. Individual assignment grades will be used to 
 * determine course grades. 
 * 
 * Provides declarations only; see Assignment.cpp for implementations.
 */

#include <string>       // for string variables
#include <chrono>       // for date and time-related variables
#include <iostream>     // for i/o streams

/**
 * @class Assignment
 * @brief Represents an academic assignment given to a student.
 * 
 * Stores metadata such as title, description, due date, completion status, and grade.
 * The Assignment object grades will be used by the Course class to determine a course grade based on the grading
 * scale. The Assignment class is used by the main function to track progress and report assignment results.
 */
class Assignment {
    private:
        std::string id_{};  // UUID v4 automatically generated during construction
        std::string title_{};
        std::string description_{};
        std::chrono::year_month_day dueDate_{};
        bool completed_{false};
        float grade_{0.0f};

        void validateGrade(float grade);

    public:
        Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate, 
            bool completed = false, float grade = 0.0);

        std::string getId() const;
        std::string getTitle() const;
        std::string getDescription() const;
        std::chrono::year_month_day getDueDate() const;
        bool getCompleted() const;
        float getGrade() const;
        void setTitle(std::string newTitle);
        void setDescription(std::string newDescription);
        void setDueDate(std::chrono::year_month_day newDueDate);
        void setCompleted(bool newCompleted);
        void setGrade(float newGrade);

        void printAssignmentInfo(std::ostream &os = std::cout) const;

        bool operator==(const Assignment &other) const;
};

#endif  // ASSIGNMENT_HPP