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

// throws an exception if a grade is less than 0 or greater than 150
void Assignment::validateGrade(float grade) {
    if (grade < 0.0f || grade > 150.0f)
        throw std::out_of_range("Grade must be from 0 to 150.");
}

Assignment::Assignment(std::string title, std::string description, std::string category, 
    std::chrono::year_month_day dueDate, bool completed, float grade) : id_{utils::generateUuid()} {
    // internal defaulting for user input
    if (dueDate == std::chrono::year_month_day{}) {
        dueDate = utils::getTodayDate();
    }

    // input validation before moving to member variables
    utils::validateReqString(title, "Title");
    utils::validateReqString(category, "Category");
    utils::validateDate(dueDate);
    validateGrade(grade);

    title_ = std::move(title);
    if (!utils::isOnlyWhitespace(description)) {
        description_ = std::move(description);
    }
    category_ = std::move(category);
    dueDate_ = dueDate;
    completed_ = completed;
    if (completed_) {
        grade_ = utils::floatRound(grade, 2);   // round to 2 decimal places for consistency
    } else {
        grade_ = 0.0f;  // explicit declaration of default
    }
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

std::string Assignment::getCategory() const {
    return category_;
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
    utils::validateReqString(newTitle, "Title");
    title_ = newTitle;
}

void Assignment::setDescription(std::string newDescription) {
    description_ = newDescription;
}

void Assignment::setCategory(std::string newCategory) {
    utils::validateReqString(newCategory, "Category");
    category_ = newCategory;
}

void Assignment::setDueDate(std::chrono::year_month_day newDueDate) {
    utils::validateDate(newDueDate);
    dueDate_ = newDueDate;
}

void Assignment::setCompleted(bool newCompleted) {
    completed_ = newCompleted;
}

// overload for percentage-based grading; simple assignment to member variable
void Assignment::setGrade(float newGrade) {
    validateGrade(newGrade);
    grade_ = utils::floatRound(newGrade, 2);    // round to 2 decimal places for consistency
}

// overload for point-based grading; need to calculate percentage before assignment
void Assignment::setGrade(float pointsEarned, float totalPoints) {
    if (totalPoints <= 0.0f) {
        throw std::invalid_argument("Total points must be greater than 0.");
    }

    float calculatedGrade = (pointsEarned / totalPoints) * 100.0f;
    setGrade(calculatedGrade);
}

// prints information held by an Assignment object
void Assignment::printAssignmentInfo(std::ostream &os) const {
    os << "ID: " << id_ << "\n";
    os << "Assignment: " << title_ << "\n";
    if (!description_.empty()) {
        os << "Description: " << description_ << "\n";
    };
    os << "Category: " << category_ << "\n";
    os << "Due Date: " << dueDate_ << "\n";
    os << "Completed? " << utils::boolToString(completed_) << "\n";
    os << "Grade: " << grade_ << "\n";
}

// equality comparison based on unique identifier (UUID)
bool Assignment::operator==(const Assignment &other) const {
    return id_ == other.id_;
}