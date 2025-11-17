#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

// -------------------------------------------------------------------------------------------------------------------
// File: Assignment.h
// description_:
//      Definition of an Assignment class that stores metadata and is used to organize coursework. There can be
//      multiple Assignments within one Course. Individual assignment grades will be used to determine course grades.
//      Provides declarations only; see Assignment.cpp for implementations.
// -------------------------------------------------------------------------------------------------------------------

#include <string>       // for string variables
#include <chrono>       // for date and time-related variables
#include "Course.hpp"   // for connection to relevant Course

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
        std::string title_{};
        std::string description_{};
        std::chrono::system_clock::time_point dueDate{};
        bool completionStatus_{false};
        float grade_{0.0};
        Course course_{};

    public:
        Assignment();
        Assignment(std::string title_, std::chrono::system_clock::time_point dueDate_);
        Assignment(std::string title_, std::chrono::system_clock::time_point dueDate_, bool completionStatus_);
        Assignment(std::string title_, std::chrono::system_clock::time_point dueDate_, bool completionStatus_, float grade_);
        Assignment(std::string title_, std::string description_, std::chrono::system_clock::time_point dueDate_);
        Assignment(std::string title_, std::string description_, std::chrono::system_clock::time_point dueDate_, bool completionStatus_);
        Assignment(std::string title_, std::string description_, std::chrono::system_clock::time_point dueDate_, bool completionStatus_, float grade_);
        Assignment(std::string title_, std::string description_, std::chrono::system_clock::time_point dueDate_, bool completionStatus_, float grade_,
                    Course course_);

        void printAssignmentInfo();

        void setTitle(std::string_view newTitle);
        void setDescription(std::string_view newDescription);
        void setDueDate(std::chrono::system_clock::time_point newDueDate);
        void setCompletionStatus(bool newCompletionStatus);
        void setGrade(float newGrade);
        void setCourse(Course newCourse);
        std::string getTitle();
        std::string getDescription();
        std::chrono::system_clock::time_point getDueDate();
        bool getCompletionStatus();
        float getGrade();
        Course getCourse();
};

#endif