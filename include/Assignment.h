#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

// -------------------------------------------------------------------------------------------------------------------
// File: Assignment.h
// description_:
//      Definition of an Assignment class that stores metadata and is used to organize coursework.
//      Provides declarations only; see Assignment.cpp for implementations.
// -------------------------------------------------------------------------------------------------------------------

#include <string>       // for std::string
#include <chrono>       // for date and time-related variables

/**
 * @class Assignment
 * @brief Represents an academic assignment given to a student.
 * 
 * Stores metadata such as title, description, due date, completion status, and grade.
 * The Assignment class is used by the main function to track progress and report assignment results.
 */
class Assignment {
    private:
        std::string title_{};
        std::string description_{};
        std::chrono::system_clock::time_point dueDate{};
        bool completionStatus_{false};
        float grade_{0.0};

    public:
        Assignment();
        Assignment(std::string title_, std::chrono::system_clock::time_point dueDate_);
        Assignment(std::string title_, std::chrono::system_clock::time_point dueDate_, bool completionStatus_);
        Assignment(std::string title_, std::chrono::system_clock::time_point dueDate_, bool completionStatus_, float grade_);

        void printAssignment();

        void setTitle(std::string_view newTitle);
        void setDescription(std::string_view newDescription);
        void setDueDate(std::chrono::system_clock::time_point newDueDate);
        void setCompletionStatus(bool newCompletionStatus);
        void setGrade(float newGrade);
        std::string getTitle();
        std::string getDescription();
        std::chrono::system_clock::time_point getDueDate();
        bool getCompletionStatus();
        float getGrade();
};

#endif