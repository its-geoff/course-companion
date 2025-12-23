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
#include <map>              // for weights and GPA values
#include <array>            // for array of gradeScale
#include "models/Assignment.hpp"   // for usage of Assignment objects in vector

/**
 * @class Course
 * @brief Represents an academic course that can contain multiple assignments at a given time.
 * 
 * Stores metadata such as title, description, start date, end date, list of assignments, number of credits,
 * and course grade.
 * The Course class can contain multiple Assignment objects. The grade of these Assignment objects will be combined
 * based on the grade weights to produce the course grade. The Course class is used by the main function to group
 * assignments and provide a student's grade for one course. 
 */
class Course {
    private:
        std::string title_{};
        std::string description_{};
        std::chrono::year_month_day startDate_{};
        std::chrono::year_month_day endDate_{};
        std::unordered_map<std::string, Assignment> assignmentList_{};
        static std::unordered_map<std::string, float> gradeWeightsDefault_;   // default grade weights if not set
        std::unordered_map<std::string, float> gradeWeights_{gradeWeightsDefault_}; // weights of each assignment type
        int numCredits_{0};
        float gradePct_{0.0};   // grade percentage from 0 to 100%
        std::string letterGrade_{};
        float gpaVal_{0.0};     // GPA value associated with letter grade
        bool active_{true};     // indicates whether the course is currently ongoing
        static std::map<float, std::string> gradeScale_;     // lower grade thresholds for letter grades
        static std::unordered_map<std::string, float> gpaScale_;       // GPA values based on letter grades

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

        std::string_view getTitle() const;
        std::string_view getDescription() const;
        std::chrono::year_month_day getStartDate() const;
        std::chrono::year_month_day getEndDate() const;
        std::unordered_map<std::string, Assignment> getAssignmentList() const;
        std::unordered_map<std::string, float> getGradeWeights() const;
        int getNumCredits() const;
        float getGradePct() const;
        std::string getLetterGrade() const;
        float getGpaVal() const;
        bool getActive() const;
        void setTitle(std::string newTitle);
        void setDescription(std::string newDescription);
        void setStartDate(std::chrono::year_month_day newStartDate);
        void setEndDate(std::chrono::year_month_day newEndDate);
        void setGradeWeights(std::unordered_map<std::string, float> newGradeWeights);     // ** TESTING
        void setNumCredits(int newNumCredits);
        void setGradePct(float newGradePct);
        // calculateLetterGrade is used in setLetterGrade
        std::string calculateLetterGrade(float gradePct, std::map<float, std::string>& gradeScale = gradeScale_);
        void setLetterGrade();
        // calculateGpaVal is used in setGpaVal
        float calculateGpaVal(std::string letterGrade);
        void setGpaVal();
        void setActive(bool newActive);

        void validateGradeWeights(std::unordered_map<std::string, float> gradeWeights);   // ** TESTING
        void validateNumCredits(int numCredits);
        void validateGradePct(float gradePct);
        void printCourseInfo(std::ostream &os);
        void addAssignment(const Assignment &assignment);
        void removeAssignment(const Assignment &assignment);

        bool operator==(const Course &other) const;
};

#endif