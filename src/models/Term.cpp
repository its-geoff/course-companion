#include "models/Term.hpp"

/**
 * @file Term.cpp
 * @brief Implementation of a Term class that stores term metadata and is used to organize courses. 
 * 
 * There can be multiple Assignments within one Course. There can be multiple Courses within one Term.
 * Course grades will be weighted based on credits to determine term GPA. Provides implementations only; 
 * see Term.hpp for definitions.
 */

#include <stdexcept>            // for exceptions
#include <cmath>                // for round
#include <iomanip>              // 
#include "app/utils.hpp"        // for reused custom functions

using namespace std::chrono_literals;

// calculates total credits based on courseList
int Term::calculateTotalCredits() {
    int result = 0;

    for (const auto& [id, course] : courseList_) {
        result += course.getNumCredits();
    }

    return result;
}

// calculates overall GPA based on courseList
float Term::calculateOvrGpa() {
    float totalGpa = 0.0;
    int credits = calculateTotalCredits();  // ensure that totalCredits is set

    for (const auto& [id, course] : courseList_) {
        totalGpa += course.getGpaVal();
    }

    return totalGpa / credits;
}

Term::Term(std::string title, std::chrono::year_month_day startDate, std::chrono::year_month_day 
    endDate) : id_{utils::generateUuid()} {
    utils::validateTitle(title);
    utils::validateDate(startDate);
    utils::validateDate(endDate);

    title_ = std::move(title);
    startDate_ = startDate;
    endDate_ = endDate;
}

Term::Term(std::string title, std::chrono::year_month_day startDate, std::chrono::year_month_day 
    endDate, bool active) : Term(title, startDate, endDate) {
    active_ = active;
}

std::string Term::getId() const {
    return id_;
}

std::string Term::getTitle() const {
    return title_;
}

std::chrono::year_month_day Term::getStartDate() const {
    return startDate_;
}

std::chrono::year_month_day Term::getEndDate() const {
    return endDate_;
}

std::unordered_map<std::string, Course> Term::getCourseList() const {
    return courseList_;
}

int Term::getTotalCredits() const {
    return totalCredits_;
}

float Term::getOvrGpa() const {
    return ovrGpa_;
}

bool Term::getActive() const {
    return active_;
}

void Term::setTitle(std::string newTitle) {
    utils::validateTitle(newTitle);
    title_ = newTitle;
}

void Term::setStartDate(std::chrono::year_month_day newStartDate) {
    utils::validateDate(newStartDate);
    startDate_ = newStartDate;
}

void Term::setEndDate(std::chrono::year_month_day newEndDate) {
    utils::validateDate(newEndDate);
    endDate_ = newEndDate;
}

void Term::setActive(bool newActive) {
    active_ = newActive;
}

void Term::printTermInfo(std::ostream &os) {
    os << "===========================================================" << "\n";
    os << "ID: " << id_ << "\n";
    os << "Term: " << title_ << "\n";
    os << "Duration: " << startDate_ << " - " << endDate_ << "\n";
    os << "Total Credits: " << totalCredits_ << "\n";
    os << "Overall GPA: " << std::setprecision(2) << ovrGpa_ << "\n";   // set precision to 2 decimal places
    os << "Current? " << utils::boolToString(active_) << "\n";
    os << "===========================================================" << "\n";
}

// adds a Course to the end of the list from the given input
void Term::addCourse(const Course& course) {
    auto [it, inserted] = courseList_.emplace(course.getId(), course);

    if (!inserted) {
        throw std::logic_error("Assignment with same ID already exists.");
    }
}

// removes a Course with the specified UUID
void Term::removeCourse(const std::string& id) {
    if (courseList_.erase(id) == 0) {
        throw std::out_of_range("Course not found.");
    }
}

// finds a Course in courseList based on ID; non-mutable (read-only)
const Course& Term::findCourse(const std::string& id) const {
    auto it = courseList_.find(id);

    if (it != courseList_.end()) {
        return it->second;
    } else {
        throw std::out_of_range("Course not found.");
    }
}

// finds an Course in courseList based on ID; mutable (read and write access)
Course& Term::findCourse(const std::string& id) {
    // use const casting to use the same logic as the const version without duplication
    const Term &selfConst = *this;
    return const_cast<Course&>(selfConst.findCourse(id));
}

bool Term::operator==(const Term &other) const {
    return id_ == other.id_;
}