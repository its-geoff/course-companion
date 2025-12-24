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
#include <algorithm>            // for vector erase
#include "app/utils.hpp"        // for reused custom functions

using namespace std::chrono_literals;

// default grade weights if unset; must add up to 1.0
std::unordered_map<std::string, float> Course::gradeWeightsDefault_ = {
    {"Homework", 0.25},
    {"Midterm", 0.35},
    {"Final Exam", 0.4}
};

// associates each percentage range to a letter grade
std::map<float, std::string> Course::gradeScale_ = {
    {97.0, "A+"},
    {93.0, "A"},
    {90.0, "A-"},
    {87.0, "B+"},
    {83.0, "B"},
    {80.0, "B-"},
    {77.0, "C+"},
    {73.0, "C"},
    {70.0, "C-"},
    {67.0, "D+"},
    {63.0, "D"},
    {60.0, "D-"},
    {0.0, "F"}
};

// associates each letter grade with a GPA value
std::unordered_map<std::string, float> Course::gpaScale_ = {
    {"A+", 4.0},
    {"A", 4.0},
    {"A-", 3.7},
    {"B+", 3.3},
    {"B", 3.0},
    {"B-", 2.7},
    {"C+", 2.3},
    {"C", 2.0},
    {"C-", 1.7},
    {"D+", 1.3},
    {"D", 1.0},
    {"D-", 0.7},
    {"F", 0.0}
};

Course::Course(std::string title, std::chrono::year_month_day startDate, std::chrono::year_month_day 
    endDate) {
    utils::validateTitle(title);
    utils::validateDate(startDate);
    utils::validateDate(endDate);

    title_ = title;
    startDate_ = startDate;
    endDate_ = endDate;
}

Course::Course(std::string title, std::chrono::year_month_day startDate, std::chrono::year_month_day 
    endDate, int numCredits) : Course(title, startDate, endDate) {
    validateNumCredits(numCredits);

    numCredits_ = numCredits;
}

Course::Course(std::string title, std::chrono::year_month_day startDate, std::chrono::year_month_day 
    endDate, int numCredits, bool active) : Course(title, startDate, endDate, numCredits) {
    active_ = active;
}

Course::Course(std::string title, std::string description, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate) {
    utils::validateTitle(title);
    utils::validateDate(startDate);
    utils::validateDate(endDate);

    title_ = title;
    description_ = description;
    startDate_ = startDate;
    endDate_ = endDate;
}

Course::Course(std::string title, std::string description, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate, 
    int numCredits) : Course(title, description, startDate, endDate) {
    validateNumCredits(numCredits);

    numCredits_ = numCredits;
}

Course::Course(std::string title, std::string description, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate, 
    int numCredits, bool active) : Course(title, description, startDate, endDate, numCredits) {
    active_ = active;
}

std::string_view Course::getTitle() const {
    return title_;
}

std::string_view Course::getDescription() const {
    return description_;
}

std::chrono::year_month_day Course::getStartDate() const {
    return startDate_;
}

std::chrono::year_month_day Course::getEndDate() const {
    return endDate_;
}

// std::unordered_map<std::string, Assignment> Course::getAssignmentList() const {
//     return assignmentList_;
// }

std::unordered_map<std::string, float> Course::getGradeWeights() const {
    return gradeWeights_;
}

int Course::getNumCredits() const {
    return numCredits_;
}

float Course::getGradePct() const {
    return gradePct_;
}

std::string Course::getLetterGrade() const {
    return letterGrade_;
}

float Course::getGpaVal() const {
    return gpaVal_;
}

bool Course::getActive() const {
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


void Course::setGradeWeights(std::unordered_map<std::string, float> newGradeWeights) {
    validateGradeWeights(newGradeWeights);
    gradeWeights_ = newGradeWeights;
}

void Course::setNumCredits(int newNumCredits) {
    validateNumCredits(newNumCredits);
    numCredits_ = newNumCredits;
}

void Course::setGradePct(float newGradePct) {
    validateGradePct(newGradePct);
    gradePct_ = newGradePct;
}

// function is used in setLetterGrade
// calculate letter grade based on grade scale
// TO-DO: allow the user to change the grading scale
std::string Course::calculateLetterGrade(float gradePct, std::map<float, std::string>& gradeScale) {
    auto iter = gradeScale.upper_bound(gradePct);
    --iter;

    return iter->second;
}

void Course::setLetterGrade() {
    letterGrade_ = calculateLetterGrade(gradePct_);
}

// function is used in setGpaVal
// calculate GPA value based on the letter grade
float Course::calculateGpaVal(std::string letterGrade) {
    return gpaScale_.at(letterGrade);
}

void Course::setGpaVal() {
    if (letterGrade_ == "") {
        setLetterGrade();
    }
    
    gpaVal_ = calculateGpaVal(letterGrade_);
}

void Course::setActive(bool newActive) {
    active_ = newActive;
}

// throws an exception if the grade weights do not add up to 1.0
void Course::validateGradeWeights(std::unordered_map<std::string, float> gradeWeights) {
    float total{0.0f};

    for (const auto& [category, weight] : gradeWeights) {
        total += weight;
    }

    if (!utils::floatEqual(total, 1.0f))
        throw std::invalid_argument("Grade weights must equal 100%.\nCurrent total: " + std::to_string(total * 100) + "%");
}

// throws an exception if numCredits is less than 0
void Course::validateNumCredits(int numCredits) {
    if (numCredits < 0) {
        throw std::out_of_range("Number of credits must be greater than or equal to 0.");
    }
}

// throws an exception if a grade is less than 0 or greater than 100
// TO-DO: enable functionality for extra credit (> 100%)
void Course::validateGradePct(float gradePct) {
    if (gradePct < 0.0f || gradePct > 100.0f)
        throw std::out_of_range("Grade percentage must be from 0 to 100.");
}

// prints information held by a Course object
void Course::printCourseInfo(std::ostream &os) {
    os << "===========================================================" << std::endl;
    os << "Course Title: " << title_ << std::endl;
    if (!description_.empty()) {
        os << "Description: " << description_ << std::endl;
    };
    os << "Duration: " << startDate_ << " - " << endDate_ << std::endl;
    os << "Number of Credits: " << numCredits_ << std::endl;
    // os << "Grade Percentage: " << gradePct_ << std::endl;
    os << "GPA Value: " << gpaVal_ << std::endl;
    os << "Current? " << utils::boolToString(active_) << std::endl;
    os << "===========================================================" << std::endl;
}

// // adds an Assignment to the end of the list from the given input
// void Course::addAssignment(const Assignment &assignment) {
//     assignmentList_.push_back(assignment);
// }

// // removes an Assignment with the specified name
// void Course::removeAssignment(const Assignment &assignment) {
//     std::erase(assignmentList_, assignment);
// }

// equality comparison based on all relevant Course fields
bool Course::operator==(const Course &other) const {
    return title_ == other.title_
        && description_ == other.description_
        && startDate_ == other.startDate_
        && endDate_ == other.endDate_
        && numCredits_ == other.numCredits_
        && active_ == other.active_;
}