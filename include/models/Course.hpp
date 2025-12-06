#ifndef COURSE_H
#define COURSE_H

/**
 * @file Course.hpp
 * @brief Definition of an Course class that stores metadata and contains zero or more Assignments. 
 * 
 * Each course has a grade that is calculated based on the assignment grades. There can be multiple Courses 
 * within a Term. Provides declarations only; see Course.cpp for implementations.
 */

#include <string>           // for string variables
#include <chrono>           // for date and time-related variables
#include <vector>           // for vector of Assignments
#include <map>              // for grading scale
#include "Assignment.hpp"   // for usage of Assignment objects in vector

/**
 * @class Course
 * @brief Represents an academic course that can contain multiple assignments at a given time.
 * 
 * Stores metadata such as title, description, start date, end date, list of assignments, number of credits,
 * grading scale, and course grade.
 * The Course class can contain multiple Assignment objects. The grade of these Assignment objects will be combined
 * based on the grading scale to produce the course grade. The Course class is used by the main function to group
 * assignments and provide a student's grade for one course. 
 */
class Course {
    private:
        std::string title_{};
        std::string description_{};
        std::chrono::system_clock::time_point startDate_{};
        std::chrono::system_clock::time_point endDate_{};
        std::vector<Assignment> assignmentList_{};
        int numCredits_{0};
        std::map<std::string, float> gradingScale_{};
        std::string grade_{};
        bool active_{true};     // indicates whether the course is currently ongoing

    public:
        Course();
        Course(std::string title_, std::chrono::system_clock::time_point startDate_, std::chrono::system_clock::time_point endDate_);
        Course(std::string title_, std::chrono::system_clock::time_point startDate_, std::chrono::system_clock::time_point endDate_,
                int numCredits_);
        Course(std::string title_, std::chrono::system_clock::time_point startDate_, std::chrono::system_clock::time_point endDate_,
                int numCredits_, bool active_);
        Course(std::string title_, std::string description_, std::chrono::system_clock::time_point startDate_, 
                std::chrono::system_clock::time_point endDate_);
        Course(std::string title_, std::string description_, std::chrono::system_clock::time_point startDate_, 
            std::chrono::system_clock::time_point endDate_, int numCredits_);
        Course(std::string title_, std::string description_, std::chrono::system_clock::time_point startDate_, 
            std::chrono::system_clock::time_point endDate_, int numCredits_, bool active_);

        void printCourseInfo();

        void setTitle(std::string_view newTitle);
        void setDescription(std::string_view newDescription);
        void setStartDate(std::chrono::system_clock::time_point newStartDate);
        void setEndDate(std::chrono::system_clock::time_point newEndDate);
        void setNumCredits(int newNumCredits);
        void setGradingScale(std::map<std::string, float> newGradingScale);
        void setActive(bool newActive);
        std::string_view getTitle();
        std::string_view getDescription();
        std::chrono::system_clock::time_point getStartDate();
        std::chrono::system_clock::time_point getEndDate();
        int getNumCredits();
        std::map<std::string, float> getGradingScale();
        bool getActive();
};

#endif