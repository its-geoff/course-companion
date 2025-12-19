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
#include "models/Assignment.hpp"   // for usage of Assignment objects in vector

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
        std::chrono::year_month_day startDate_{};
        std::chrono::year_month_day endDate_{};
        std::vector<Assignment> assignmentList_{};
        int numCredits_{0};
        std::map<std::string, float> gradingScale_{};
        float gradePct_{0.0};    // grade percentage from 0 to 100%
        std::string letterGrade_{};
        bool active_{true};     // indicates whether the course is currently ongoing

    public:
        Course(std::string title, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate);
        Course(std::string title, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate,
                int numCredits);
        Course(std::string title, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate,
                int numCredits, bool active);
        Course(std::string title, std::string description, std::chrono::year_month_day startDate, 
                std::chrono::year_month_day endDate);
        Course(std::string title, std::string description, std::chrono::year_month_day startDate, 
            std::chrono::year_month_day endDate, int numCredits);
        Course(std::string title, std::string description, std::chrono::year_month_day startDate, 
            std::chrono::year_month_day endDate, int numCredits, bool active);

        std::string_view getTitle();
        std::string_view getDescription();
        std::chrono::year_month_day getStartDate();
        std::chrono::year_month_day getEndDate();
        int getNumCredits();
        std::map<std::string, float> getGradingScale();
        float getGradePct();
        std::string getLetterGrade();
        bool getActive();
        void setTitle(std::string newTitle);
        void setDescription(std::string newDescription);
        void setStartDate(std::chrono::year_month_day newStartDate);
        void setEndDate(std::chrono::year_month_day newEndDate);
        void setNumCredits(int newNumCredits);
        void setGradePct(float newGradePct);
        void setLetterGrade(std::string newLetterGrade);
        void setGradingScale(std::map<std::string, float> newGradingScale);
        void setActive(bool newActive);

        void calculateLetterGrade();
        void printCourseInfo();
        void addAssignment();
        void removeAssignment();
};

#endif