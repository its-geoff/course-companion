#include "models/Course.hpp"

/**
 * @file Course.cpp
 * @brief Implementation of a Course class that stores course metadata and is used to organize coursework. 
 * 
 * There can be multiple Assignments within one Course. There can be multiple Courses within one Term.
 * Individual assignment grades will be weighted to determine course grades. Provides implementations only; 
 * see Course.hpp for definitions.
 */

#include <stdexcept>            // for exceptions
#include "app/utils.hpp"        // for isOnlyWhitespace and validation

using namespace std::chrono_literals;

Course::Course(std::string title, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) {
    utils::validateTitle(title);

    title_ = title;
    startDate_ = startDate;
    endDate_ = endDate;
}

Course::Course(std::string title, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate, int numCredits) {
    utils::validateTitle(title);

    title_ = title;
    startDate_ = startDate;
    endDate_ = endDate;
    numCredits_ = numCredits;
}

Course::Course(std::string title, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate, int numCredits, bool active) {
    utils::validateTitle(title);

    title_ = title;
    startDate_ = startDate;
    endDate_ = endDate;
    numCredits_ = numCredits;
    active_ = active;
}

Course::Course(std::string title, std::string description, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) {
    utils::validateTitle(title);

    title_ = title;
    description_ = description;
    startDate_ = startDate;
    endDate_ = endDate;
}

Course::Course(std::string title, std::string description, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate, int numCredits) {
    utils::validateTitle(title);

    title_ = title;
    description_ = description;
    startDate_ = startDate;
    endDate_ = endDate;
    numCredits_ = numCredits;
}

Course::Course(std::string title, std::string description, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate, int numCredits, bool active) {
    utils::validateTitle(title);

    title_ = title;
    description_ = description;
    startDate_ = startDate;
    endDate_ = endDate;
    numCredits_ = numCredits;
    active_ = active;
}

std::string_view Course::getTitle() {
    return title_;
}

std::string_view Course::getDescription() {
    return description_;
}

std::chrono::year_month_day Course::getStartDate() {
    return startDate_;
}

std::chrono::year_month_day Course::getEndDate() {
    return endDate_;
}

int Course::getNumCredits() {
    return numCredits_;
}

std::map<std::string, float> Course::getGradingScale() {
    return gradingScale_;
}

float Course::getGradePct() {
    return gradePct_;
}

std::string Course::getLetterGrade() {
    return letterGrade_;
}

bool Course::getActive() {
    return active_;
}

void Course::setTitle(std::string newTitle) {
    utils::validateTitle(newTitle);
    title_ = newTitle;
}

void Course::setDescription(std::string newDescription) {
    description_ = newDescription;
}

void Course::setStartDate(std::chrono::year_month_day newStartDate) {
    utils::validateDate(newStartDate);
    startDate_ = newStartDate;
}

void Course::setEndDate(std::chrono::year_month_day newEndDate) {
    utils::validateDate(newEndDate);
    endDate_ = newEndDate;
}

void Course::setNumCredits(int newNumCredits) {
    numCredits_ = newNumCredits;
}

void Course::setGradePct(float newGradePct) {
    gradePct_ = newGradePct;
}

void Course::setLetterGrade(std::string newLetterGrade) {
    letterGrade_ = newLetterGrade;
}

void Course::setGradingScale(std::map<std::string, float> newGradingScale) {
    gradingScale_ = newGradingScale;
}

void Course::setActive(bool newActive) {
    active_ = newActive;
}

// // adds an Assignment using the given user input
// // evaluate if this is needed or if this should be in Course
// void addAssignment() {

// }

// void removeAssignment() {

// }