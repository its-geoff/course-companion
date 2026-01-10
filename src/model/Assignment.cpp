#include "model/Assignment.hpp"

/**
 * @file Assignment.cpp
 * @brief Implementation of an Assignment class, which stores metadata and is used to organize coursework. 
 * 
 * There can be multiple Assignments within one Course. Individual assignment grades will be used to 
 * determine course grades. 
 * 
 * Provides implementations only; see Assignment.hpp for definitions.
 */

#include <stdexcept>            // for exceptions
#include "utils/utils.hpp"        // for reused custom functions

using namespace std::chrono_literals;

Assignment::Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate, 
    bool completed, float grade) : id_{utils::generateUuid()} {
    // internal defaulting for user input
    if (dueDate == std::chrono::year_month_day{}) {
        dueDate = utils::getTodayDate();
    }

    // input validation before moving to member variables
    utils::validateTitle(title);
    utils::validateDate(dueDate);
    validateGrade(grade);

    title_ = std::move(title);
    if (!utils::isOnlyWhitespace(description)) {
        description_ = std::move(description);
    }
    dueDate_ = dueDate;
    completed_ = completed;
    grade_ = grade;
}

std::string Assignment::getId() const {
    return id_;
}

std::string Assignment::getTitle() const {
    return title_;
}

std::string Assignment::getDescription() const {
    return description_;
}

std::chrono::year_month_day Assignment::getDueDate() const {
    return dueDate_;
}

bool Assignment::getCompleted() const {
    return completed_;
}

float Assignment::getGrade() const {
    return grade_;
}

void Assignment::setTitle(std::string newTitle) {
    utils::validateTitle(newTitle);
    title_ = newTitle;
}

void Assignment::setDescription(std::string newDescription) {
    description_ = newDescription;
}

void Assignment::setDueDate(std::chrono::year_month_day newDueDate) {
    utils::validateDate(newDueDate);
    dueDate_ = newDueDate;
}

void Assignment::setCompleted(bool newCompleted) {
    completed_ = newCompleted;
}

void Assignment::setGrade(float newGrade) {
    validateGrade(newGrade);
    grade_ = newGrade;
}

// throws an exception if a grade is less than 0 or greater than 100
void Assignment::validateGrade(float grade) {
    if (grade < 0.0f || grade > 100.0f)
        throw std::out_of_range("Grade must be from 0 to 100.");
}

// prints information held by an Assignment object
void Assignment::printAssignmentInfo(std::ostream &os) const {
    os << "ID: " << id_ << "\n";
    os << "Assignment: " << title_ << "\n";
    if (!description_.empty()) {
        os << "Description: " << description_ << "\n";
    };
    // os << "Course: " << course_.getTitle() << "\n";  -> no Course implementation yet
    os << "Due Date: " << dueDate_ << "\n";
    os << "Completed? " << utils::boolToString(completed_) << "\n";
    os << "Grade: " << grade_ << "\n";
}

// equality comparison based on unique identifier (UUID)
bool Assignment::operator==(const Assignment &other) const {
    return id_ == other.id_;
}