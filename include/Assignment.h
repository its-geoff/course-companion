#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

// -------------------------------------------------------------------------------------------------------------------
// File: Assignment.h
// Description:
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
        std::string title{};
        std::string description{};
        std::chrono::system_clock::time_point due_date;
        bool completion_status{false};
        float grade{0.0};

    public:
};

#endif