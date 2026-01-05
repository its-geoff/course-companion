#ifndef TERM_HPP
#define TERM_HPP

/**
 * @file Term.hpp
 * @brief Definition of the Term class, which stores metadata and contains zero or more Courses. 
 * 
 * Each term has a GPA that is calculated based on the course grades. 
 * 
 * Provides declarations only; see Term.cpp for implementations.
 */

#include <string>           // for string variables
#include <chrono>           // for date and time-related variables
#include <iostream>         // for i/o streams
#include <unordered_map>    // for courseList
#include "model/Course.hpp"   // for usage of Course objects in vector

/**
 * @class Term
 * @brief Represents an academic term that can contain multiple courses at a given time.
 * 
 * Stores metadata such as title, start date, end date, list of courses, and term GPA.
 * The Term class can contain multiple Course objects. The grade of these Course objects will be combined
 * based on the number of credits to produce the term GPA. The Term class is used by the main function to group
 * courses and provide a student's grade for one term. 
 */
class Term {
    private:
        std::string id_{};  // UUID v4 automatically generated during construction
        std::string title_{};
        std::chrono::year_month_day startDate_{};
        std::chrono::year_month_day endDate_{};
        std::unordered_map<std::string, Course> courseList_{};  // id -> Course
        int totalCredits_{0};
        float ovrGpa_{0.0};
        bool active_{true}; // indicates whether the term is currently ongoing

        int calculateTotalCredits();    // calculates total credits - need to auto call during add or remove Course
        float calculateOvrGpa();        // calculates overall gpa - need to auto call during add or remove Course

    public:
        Term(std::string title, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate,
            bool active = true);

        std::string getId() const;
        std::string getTitle() const;
        std::chrono::year_month_day getStartDate() const;
        std::chrono::year_month_day getEndDate() const;
        const std::unordered_map<std::string, Course>& getCourseList() const;
        int getTotalCredits() const;
        float getOvrGpa() const;
        bool getActive() const;
        void setTitle(std::string newTitle);
        void setStartDate(std::chrono::year_month_day newStartDate);
        void setEndDate(std::chrono::year_month_day newEndDate);
        void setActive(bool newActive);

        void printTermInfo(std::ostream &os = std::cout);
        void addCourse(const Course& course);
        void removeCourse(const std::string& id);
        const Course& findCourse(const std::string& id) const;    // non-mutable version
        Course& findCourse(const std::string& id);    // mutable version

        bool operator==(const Term &other) const;
};

#endif  // TERM_HPP