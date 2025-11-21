#ifndef TERM_H
#define TERM_H

// -------------------------------------------------------------------------------------------------------------------
// File: Term.hpp
// description_:
//      Definition of an Term class that stores metadata and contains zero or more Courses. Each term has a GPA that
//      is calculated based on the course grades.
//      Provides declarations only; see Term.cpp for implementations.
// -------------------------------------------------------------------------------------------------------------------

#include <string>           // for string variables
#include <chrono>           // for date and time-related variables
#include <vector>           // for vector of Courses
#include "Course.hpp"   // for usage of Course objects in vector

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
        std::string title_{};
        std::chrono::system_clock::time_point startDate_{};
        std::chrono::system_clock::time_point endDate_{};
        std::vector<Course> courseList_{};
        float gpa_{0.0};
        bool active_{true};     // indicates whether the term is currently ongoing

    public:
        Term();
        Term(std::string title_, std::chrono::system_clock::time_point startDate_, std::chrono::system_clock::time_point endDate_);
        Term(std::string title_, std::chrono::system_clock::time_point startDate_, std::chrono::system_clock::time_point endDate_,
                std::vector<Course> courseList_);
        Term(std::string title_, std::chrono::system_clock::time_point startDate_, std::chrono::system_clock::time_point endDate_,
                std::vector<Course> courseList_, bool active_);

        void printTermInfo();

        void setTitle(std::string newTitle);
        void setStartDate(std::chrono::system_clock::time_point newStartDate);
        void setEndDate(std::chrono::system_clock::time_point newEndDate);
        void setActive(bool newActive);
        std::string_view getTitle();
        std::chrono::system_clock::time_point getStartDate();
        std::chrono::system_clock::time_point getEndDate();
        bool getActive();
};

#endif