#ifndef COURSE_HPP
#define COURSE_HPP

/**
 * @file Course.hpp
 * @brief Definition of the Course class, which stores metadata and contains zero or more Assignments. 
 * 
 * Each course has a grade that is calculated based on the assignment grades. There can be multiple Courses 
 * within a Term. 
 * 
 * Provides declarations only; see Course.cpp for implementations.
 */

#include <string>           // for string variables
#include <chrono>           // for date and time-related variables
#include <iostream>         // for i/o streams
#include <map>              // for GPA scale
#include <unordered_map>    // for weights, GPA values, and assignmentList
#include "model/Assignment.hpp"   // for usage of Assignment objects in Course

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
        std::string id_{};  // UUID v4 automatically generated during construction
        std::string title_{};
        std::string description_{};
        std::chrono::year_month_day startDate_{};
        std::chrono::year_month_day endDate_{};
        std::unordered_map<std::string, Assignment> assignmentList_{};  // id -> Assignment
        static const std::unordered_map<std::string, float> gradeWeightsDefault_;   // default grade weights if not set
        std::unordered_map<std::string, float> gradeWeights_{gradeWeightsDefault_}; // weights of each assignment category
        std::unordered_map<std::string, float> gradesByCategory_;   // map of each raw percentage grade by category
        int numCredits_{3};     // default number of credits for a class, TO-DO: allow user to override default
        float gradePct_{0.0};   // grade percentage from 0 to 100%
        std::string letterGrade_{"N/A"};
        float gpaVal_{0.0};     // GPA value associated with letter grade
        bool active_{true};     // indicates whether the course is currently ongoing
        static const std::map<float, std::string> gradeScaleDefault_;    // default grade scale if not set
        std::map<float, std::string> gradeScale_{gradeScaleDefault_};     // lower grade thresholds for letter grades
        static const std::unordered_map<std::string, float> gpaScale_;       // GPA values based on letter grades

        void validateGradeWeights(const std::unordered_map<std::string, float>& gradeWeights);
        void validateNumCredits(int numCredits);
        void validateGradePct(float gradePct);
        void validateGradeScale(const std::map<float, std::string>& gradeScale);

        void calculateGradesByCategory();
        float calculateGradePct();
        std::string calculateLetterGrade(float gradePct, const std::map<float, std::string>& gradeScale) const; // auto set during add or remove Assignment?
        std::string calculateLetterGrade(float gradePct) const;
        float calculateGpaVal(const std::string& letterGrade);
        int calculateCompletedAssignments() const;

    public:
        Course(std::string title, std::string description, std::chrono::year_month_day startDate, 
            std::chrono::year_month_day endDate, int numCredits = 3, bool active = true);

        std::string getId() const;
        std::string getTitle() const;
        std::string getDescription() const;
        std::chrono::year_month_day getStartDate() const;
        std::chrono::year_month_day getEndDate() const;
        const std::unordered_map<std::string, Assignment>& getAssignmentList() const;
        const std::unordered_map<std::string, float>& getGradeWeights() const;
        int getNumCredits() const;
        float getGradePct() const;
        std::string getLetterGrade() const;
        float getGpaVal() const;
        bool getActive() const;
        std::map<float, std::string> getGradeScale() const;     // need to test
        void setTitle(std::string newTitle);
        void setDescription(std::string newDescription);
        void setStartDate(std::chrono::year_month_day newStartDate);
        void setEndDate(std::chrono::year_month_day newEndDate);
        void setGradeWeights(const std::unordered_map<std::string, float>& newGradeWeights);
        void setNumCredits(int newNumCredits);
        void setGradePct();
        void setGradePct(float newGradePct);
        void setGradeScale(const std::map<float, std::string>& newGradeScale);      // need to test
        void setLetterGrade();
        void setGpaVal();
        void setActive(bool newActive);

        void printCourseInfo(std::ostream& os = std::cout) const;
        void addAssignment(const Assignment& assignment);
        void removeAssignment(const std::string& id);
        const Assignment& findAssignment(const std::string& id) const;    // non-mutable version
        Assignment& findAssignment(const std::string& id);    // mutable version

        bool operator==(const Course &other) const;
};

#endif  // COURSE_HPP