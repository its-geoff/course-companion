#include "models/Assignment.hpp"

/**
 * @file Assignment.cpp
 * @brief Implementation of an Assignment class that stores metadata and is used to organize coursework. 
 * 
 * There can be multiple Assignments within one Course. Individual assignment grades will be used to 
 * determine course grades. Provides implementations only; see Assignment.hpp for definitions.
 */

#include <stdexcept>            // for exceptions
#include "app/utils.hpp"        // for reused custom functions

using namespace std::chrono_literals;

Assignment::Assignment(std::string title) : id_{utils::generateUuid()} {
    utils::validateTitle(title);

    title_ = std::move(title);
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::time_point_cast<std::chrono::days>(now);    // set to today
    dueDate_ = std::chrono::year_month_day(today);
}

Assignment::Assignment(std::string title, std::chrono::year_month_day dueDate) : 
    Assignment(title) {
    utils::validateDate(dueDate);

    dueDate_ = dueDate;
}

Assignment::Assignment(std::string title, std::chrono::year_month_day dueDate, bool completed)
    : Assignment(title, dueDate) {
    completed_ = completed;
}

Assignment::Assignment(std::string title, std::chrono::year_month_day dueDate, bool completed, 
    float grade) : Assignment(title, dueDate, completed) {
    validateGrade(grade);

    grade_ = grade;
}

Assignment::Assignment(std::string title, std::string description) : id_{utils::generateUuid()} {
    utils::validateTitle(title);

    title_ = std::move(title);
    description_ = std::move(description);
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::time_point_cast<std::chrono::days>(now);    // set to today
    dueDate_ = std::chrono::year_month_day(today);
}

Assignment::Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate)
    : Assignment(title, description) {
    utils::validateDate(dueDate);

    dueDate_ = dueDate;
}

Assignment::Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate, 
    bool completed) : Assignment(title, description, dueDate) {
    completed_ = completed;
}

Assignment::Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate, 
    bool completed, float grade) : Assignment(title, description, dueDate, completed) {
    validateGrade(grade);
    grade_ = grade;
}

// Assignment::Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate, bool completed, float grade,
//             Course course) {
//     title_ = title;
//     description_ = description;
//     dueDate_ = dueDate;
//     completed_ = completed;
//     grade_ = grade;
//     course_ = course;
// }  -> no Course implementation yet

std::string_view Assignment::getId() const {
    return id_;
}

std::string_view Assignment::getTitle() const {
    return title_;
}

std::string_view Assignment::getDescription() const {
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

// Course Assignment::getCourse() const {
//     return course_;
// }  -> no Course implementation yet

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

// void Assignment::setCourse(Course newCourse) {
//     course_ = newCourse;
// }  -> no Course implementation yet

// throws an exception if a grade is less than 0 or greater than 100
void Assignment::validateGrade(float grade) {
    if (grade < 0.0f || grade > 100.0f)
        throw std::out_of_range("Grade must be from 0 to 100.");
}

// prints information held by an Assignment object
void Assignment::printAssignmentInfo(std::ostream &os) {
    os << "===========================================================" << "\n";
    os << "ID: " << id_ << "\n";
    os << "Assignment Title: " << title_ << "\n";
    if (!description_.empty()) {
        os << "Description: " << description_ << "\n";
    };
    // os << "Course: " << course_.getTitle() << "\n";  -> no Course implementation yet
    os << "Due Date: " << dueDate_ << "\n";
    os << "Completed? " << utils::boolToString(completed_) << "\n";
    os << "Grade: " << grade_ << "\n";
    os << "===========================================================" << "\n";
}

// equality comparison based on unique identifier (UUID)
bool Assignment::operator==(const Assignment &other) const {
    return id_ == other.id_;
}