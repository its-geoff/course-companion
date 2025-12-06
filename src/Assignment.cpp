#include "Assignment.hpp"

/**
 * @file Assignment.hpp
 * 
 * @brief Implementation of an Assignment class that stores metadata and is used to organize coursework. There can be
 * multiple Assignments within one Course. Individual assignment grades will be used to determine course grades.
 * Provides implementations only; see Assignment.hpp for definitions.
 */

#include <stdexcept>       // for exceptions
#include <algorithm>       // for all_of
#include <cctype>          // for isspace

using namespace std::chrono_literals;

Assignment::Assignment(std::string title) {
    validateTitle(title);

    title_ = title;
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::time_point_cast<std::chrono::days>(now);    // set to today
    dueDate_ = std::chrono::year_month_day(today);
    completed_ = false;
}

Assignment::Assignment(std::string title, std::chrono::year_month_day dueDate) {
    validateTitle(title);
    validateDueDate(dueDate);

    title_ = title;
    dueDate_ = dueDate;
    completed_ = false;
}

Assignment::Assignment(std::string title, std::chrono::year_month_day dueDate, bool completed) {
    validateTitle(title);
    validateDueDate(dueDate);
    
    title_ = title;
    dueDate_ = dueDate;
    completed_ = completed;
}

Assignment::Assignment(std::string title, std::chrono::year_month_day dueDate, bool completed, float grade) {
    validateTitle(title);
    validateDueDate(dueDate);
    validateGrade(grade);

    title_ = title;
    dueDate_ = dueDate;
    completed_ = completed;
    grade_ = grade;
}

Assignment::Assignment(std::string title, std::string description) {
    validateTitle(title);

    title_ = title;
    description_ = description;
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::time_point_cast<std::chrono::days>(now);    // set to today
    dueDate_ = std::chrono::year_month_day(today);
    completed_ = false;
}

Assignment::Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate) {
    validateTitle(title);
    validateDueDate(dueDate);

    title_ = title;
    description_ = description;
    dueDate_ = dueDate;
    completed_ = false;
}

Assignment::Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate, bool completed) {
    validateTitle(title);
    validateDueDate(dueDate);
    
    title_ = title;
    description_ = description;
    dueDate_ = dueDate;
    completed_ = completed;
}

Assignment::Assignment(std::string title, std::string description, std::chrono::year_month_day dueDate, bool completed, float grade) {
    validateTitle(title);
    validateDueDate(dueDate);
    validateGrade(grade);
    
    title_ = title;
    description_ = description;
    dueDate_ = dueDate;
    completed_ = completed;
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

std::string_view Assignment::getTitle() {
    return title_;
}

std::string_view Assignment::getDescription() {
    return description_;
}

std::chrono::year_month_day Assignment::getDueDate() {
    return dueDate_;
}

bool Assignment::getCompleted() {
    return completed_;
}

float Assignment::getGrade() {
    return grade_;
}

// Course Assignment::getCourse() {
//     return course_;
// }  -> no Course implementation yet

void Assignment::setTitle(std::string newTitle) {
    validateTitle(newTitle);
    title_ = newTitle;
}

void Assignment::setDescription(std::string newDescription) {
    description_ = newDescription;
}

void Assignment::setDueDate(std::chrono::year_month_day newDueDate) {
    validateDueDate(newDueDate);
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

// checks if a string is only whitespace
bool Assignment::isOnlyWhitespace(const std::string_view str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char c) { return std::isspace(c); });
}

// throws an exception if a string is empty
void Assignment::validateTitle(std::string_view title) {
    if (isOnlyWhitespace(title))
        throw std::invalid_argument("Title must be non-empty.");
}

// throws an exception if a date is non-existent
void Assignment::validateDueDate(std::chrono::year_month_day dueDate) {
    if (!dueDate.ok())
        throw std::invalid_argument("Date is invalid.");
}

// throws an exception if a grade is less than 0 or greater than 100
void Assignment::validateGrade(float grade) {
    if (grade < 0.0f || grade > 100.0f)
        throw std::out_of_range("Grade must be from 0 to 100.");
}

// converts bool value of completed into a string for output
std::string Assignment::completedString(bool completed) {
    if (completed)
        return "Yes";
    else
        return "No";
}

// prints information held by an Assignment object
void Assignment::printAssignmentInfo(std::ostream &os) {
    os << "===========================================================" << std::endl;
    os << "Assignment Title: " << title_ << std::endl;
    if (!description_.empty()) {
        os << "Description: " << description_ << std::endl;
    };
    // os << "Course: " << course_.getTitle() << std::endl;  -> no Course implementation yet
    os << "Due Date: " << dueDate_ << std::endl;
    os << "Completed? " << completedString(completed_) << std::endl;
    os << "Grade: " << grade_<< std::endl;
    os << "===========================================================" << std::endl;
}

// reads optional string from user input
std::optional<std::string> Assignment::readOptionalString(std::istream &is) {
    std::string response;
    std::getline(is, response);

    if (response.empty())
        return std::nullopt;

    return response;
}

// reads optional date from user input and converts to the year_month_day format
std::optional<std::chrono::year_month_day> Assignment::readOptionalDate() {
    std::string response;
    std::getline(std::cin, response);
    if (response.empty())
        return std::nullopt;

    int y;
    unsigned int m, d;
    if (sscanf(response.c_str(), "%d-%d-%d", &y, &m, &d) != 3)
        return std::nullopt;

    std::chrono::year_month_day output{std::chrono::year{y}, std::chrono::month{m}, std::chrono::day{d}};
    if (!output.ok())
        return std::nullopt;

    return output;
}

// reads optional bool from user input and converts to the bool format
std::optional<bool> Assignment::readOptionalBool() {
    std::string response;
    std::getline(std::cin, response);
    if (response.empty())
        return std::nullopt;

    if (response == "yes" || response == "y" || response == "1")
        return true;
    else if (response == "no" || response == "n" || response == "0")
        return false;

    return std::nullopt;
}

// reads optional float from user input and converts to the float format
std::optional<float> Assignment::readOptionalFloat() {
    std::string response;
    std::getline(std::cin, response);
    if (response.empty())
        return std::nullopt;

    return std::stof(response);
}

// chooses a constructor for this class based on the user input given
Assignment Assignment::chooseConstructor(std::string title, std::optional<std::string> description, std::optional<std::chrono::year_month_day> dueDate, 
    std::optional<bool> completed, std::optional<float> grade) {
    if (description && dueDate && completed && grade)
        return Assignment(title, *description, *dueDate, *completed, *grade);
    else if (description && dueDate && completed)
        return Assignment(title, *description, *dueDate, *completed);
    else if (description && dueDate)
        return Assignment(title, *description, *dueDate);
    else if (description)
        return Assignment(title, *description);
    
    if (dueDate && completed && grade)
        return Assignment(title, *dueDate, *completed, *grade);
    else if (dueDate && completed)
        return Assignment(title, *dueDate, *completed);
    else if (dueDate)
        return Assignment(title, *dueDate);

    return Assignment(title);
}