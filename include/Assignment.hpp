#ifndef ASSIGNMENT_H
#define ASSIGNMENT_H

/**
 * @file Assignment.hpp
 * @brief Definition of an Assignment class that stores metadata and is used to organize coursework. 
 * 
 * There can be multiple Assignments within one Course. Individual assignment grades will be used to 
 * determine course grades. Provides declarations only; see Assignment.cpp for implementations.
 */

#include <string>       // for string variables
#include <chrono>       // for date and time-related variables
#include <optional>     // for optional variables
#include <iostream>     // for i/o streams
// #include "Course.hpp"   // for connection to relevant Course  -> no Course implementation yet

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
        std::chrono::year_month_day dueDate_{};
        bool completed_{false};
        float grade_{0.0f};
        // Course course_{};  -> no Course implementation yet

    public:
        Assignment(std::string title);
        Assignment(std::string title, std::chrono::year_month_day dueDate);
        Assignment(std::string title, std::chrono::year_month_day dueDate, bool completed);
        Assignment(std::string title, std::chrono::year_month_day dueDate, bool completed, float grade);
        Assignment(std::string title, std::string description);
        Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate);
        Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate, bool completed);
        Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate, bool completed, float grade);
        // Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate, bool completed, float grade,
        //             Course course);  -> no Course implementation yet

        std::string_view getTitle();
        std::string_view getDescription();
        std::chrono::year_month_day getDueDate();
        bool getCompleted();
        float getGrade();
        // Course getCourse();  -> no Course implementation yet
        void setTitle(std::string newTitle);
        void setDescription(std::string newDescription);
        void setDueDate(std::chrono::year_month_day newDueDate);
        void setCompleted(bool newCompleted);
        void setGrade(float newGrade);
        // void setCourse(Course newCourse);  -> no Course implementation yet

        bool isOnlyWhitespace(const std::string_view str);
        void validateTitle(std::string_view title);
        void validateGrade(float grade);
        void validateDueDate(std::chrono::year_month_day dueDate);
        std::string completedString(bool completed);
        void printAssignmentInfo(std::ostream &os);
        std::optional<std::string> readOptionalString(std::istream &is);
        std::optional<std::chrono::year_month_day> readOptionalDate(std::istream &is);
        std::optional<bool> readOptionalBool(std::istream &is);
        std::optional<float> readOptionalFloat(std::istream &is);
        Assignment chooseConstructor(std::string title, std::optional<std::string> description, std::optional<std::chrono::year_month_day> date, 
            std::optional<bool> completed, std::optional<float> grade);
        // next, implement addAssignment and removeAssignment
        void addAssignment();
        void removeAssignment();
};

#endif