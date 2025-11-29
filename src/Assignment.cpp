#include "Assignment.hpp"

/**
 * @file Assignment.hpp
 * 
 * @brief Implementation of an Assignment class that stores metadata and is used to organize coursework. There can be
 * multiple Assignments within one Course. Individual assignment grades will be used to determine course grades.
 * Provides implementations only; see Assignment.hpp for definitions.
 */

 #include <iostream>        // for cout

Assignment::Assignment(std::string title) {
    title_ = title;
    auto now = std::chrono::system_clock::now;
    auto today = std::chrono::floor<std::chrono::days>(now);    // set to today
    dueDate_ = today;
    completed_ = false;
}

Assignment::Assignment(std::string title, std::chrono::year_month_day dueDate) {
    title_ = title;
    dueDate_ = dueDate;
    completed_ = false;
}

Assignment::Assignment(std::string title, std::chrono::year_month_day dueDate, bool completed) {
    title_ = title;
    dueDate_ = dueDate;
    completed_ = completed;
}

Assignment::Assignment(std::string title, std::chrono::year_month_day dueDate, bool completed, float grade) {
    title_ = title;
    dueDate_ = dueDate;
    completed_ = completed;
    grade_ = grade;
}

Assignment::Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate) {
    title_ = title;
    description_ = description;
    dueDate_ = dueDate;
    completed_ = false;
}

Assignment::Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate, bool completed) {
    title_ = title;
    description_ = description;
    dueDate_ = dueDate;
    completed_ = completed;
}

Assignment::Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate, bool completed, float grade) {
    title_ = title;
    description_ = description;
    dueDate_ = dueDate;
    completed_ = completed;
    grade_ = grade;
}

Assignment::Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate, bool completed, float grade,
            Course course) {
    title_ = title;
    description_ = description;
    dueDate_ = dueDate;
    completed_ = completed;
    grade_ = grade;
    course_ = course;
}

void Assignment::printAssignmentInfo() {
    std::cout << "===========================================================";
    std::cout << "Assignment Title: " << title_ << std::endl;
    if (!description_.empty()) {
        std::cout << "Description: " << description_ << std::endl;
    };
    std::cout << "Course: " << course_.getTitle() << std::endl;
    std::cout << "Due Date: " << dueDate_ << std::endl;
    std::cout << "Completed? " << completed_ << std::endl;
    std::cout << "Grade: " << grade_<< std::endl;
    std::cout << "===========================================================";
}

void Assignment::setTitle(std::string newTitle) {
    title_ = newTitle;
}

void Assignment::setDescription(std::string newDescription) {
    description_ = newDescription;
}

void Assignment::setDueDate(std::chrono::year_month_day newDueDate) {
    dueDate_ = newDueDate;
}

void Assignment::setCompletionStatus(bool newCompletionStatus) {
    completed_ = newCompletionStatus;
}

void Assignment::setGrade(float newGrade) {
    grade_ = newGrade;
}

void Assignment::setCourse(Course newCourse) {
    course_ = newCourse;
}

std::string_view Assignment::getTitle() {
    return title_;
}

std::string_view Assignment::getDescription() {
    return description_;
}

std::chrono::year_month_day Assignment::getDueDate() {
    return dueDate_;
}

bool Assignment::getCompletionStatus() {
    return completed_;
}

float Assignment::getGrade() {
    return grade_;
}

Course Assignment::getCourse() {
    return course_;
}