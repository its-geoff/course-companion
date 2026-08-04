#ifndef ASSIGNMENT_HPP
#define ASSIGNMENT_HPP

/**
 * @file Assignment.hpp
 * @brief Definition of the Assignment class, which stores metadata and is used to organize coursework. 
 * 
 * There can be multiple Assignments within one Course. Individual assignment grades will be used to 
 * determine course grades. 
 * 
 * Provides declarations only; see Assignment.cpp for implementations.
 */

#include <string>       // for string variables
#include <chrono>       // for date and time-related variables
#include <iostream>     // for i/o streams

class Assignment {
    public:
        Assignment(std::string title, std::string description, std::string category, 
            std::chrono::year_month_day dueDate, bool completed = false, float grade = 0.0);

        std::string getId() const;
        std::string getTitle() const;
        std::string getDescription() const;
        std::string getCategory() const;
        std::chrono::year_month_day getDueDate() const;
        bool getCompleted() const;
        float getGrade() const;
        void setTitle(std::string newTitle);
        void setDescription(std::string newDescription);
        void setCategory(std::string newCategory);
        void setDueDate(std::chrono::year_month_day newDueDate);
        void setCompleted(bool newCompleted);
        void setGrade(float newGrade);  // overload for percentage-based grading
        void setGrade(float pointsEarned, float totalPoints);   // overload for point-based grading

        void printAssignmentInfo(std::ostream &os = std::cout) const;
        static Assignment fromRow(std::string id, std::string title, std::string description,
            std::string category, std::chrono::year_month_day dueDate, bool completed, float grade);

        bool operator==(const Assignment &other) const;

    private:
        std::string id_{};  // UUID v4 automatically generated during construction
        std::string title_{};
        std::string description_{};
        std::string category_{};    // validation handled in higher-level application logic
        std::chrono::year_month_day dueDate_{};
        bool completed_{false};
        float grade_{0.0f};

        void validateGrade(float grade);
};

#endif  // ASSIGNMENT_HPP