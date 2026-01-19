#include "model/Course.hpp"

/**
 * @file Course.cpp
 * @brief Implementation of the Course class, which stores course metadata and is used to organize coursework. 
 * 
 * There can be multiple Assignments within one Course. There can be multiple Courses within one Term.
 * Individual assignment grades will be weighted to determine course grades. 
 * 
 * Provides implementations only; see Course.hpp for definitions.
 */

#include <stdexcept>            // for exceptions
#include <iomanip>              // for string formatting in print statement
#include "utils/utils.hpp"        // for reused custom functions

using namespace std::chrono_literals;

// default grade weights if unset; must add up to 1.0
const std::unordered_map<std::string, float> Course::gradeWeightsDefault_ = {
    {"Homework", 0.25},
    {"Midterm", 0.35},
    {"Final Exam", 0.4}
};

// associates each percentage range to a letter grade
const std::map<float, std::string> Course::gradeScaleDefault_ = {
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
const std::unordered_map<std::string, float> Course::gpaScale_ = {
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
    {"F", 0.0},
    {"N/A", 0.0}
};

// throws an exception if the grade weights do not add up to 1.0
void Course::validateGradeWeights(const std::unordered_map<std::string, float>& gradeWeights) {
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

// throws an exception if a grade is less than 0 or greater than 150
void Course::validateGradePct(float gradePct) {
    if (gradePct < 0.0f || gradePct > 150.0f)
        throw std::out_of_range("Grade percentage must be from 0 to 150.");
}

// throws an exception if the grade scale is empty or does not provide grades for 0-100
void Course::validateGradeScale(const std::map<float, std::string>& gradeScale) {
    if (gradeScale.empty()) {
        throw std::runtime_error("Grade scale is empty.");
    }

    if (!gradeScale.contains(0.0f)) {
        throw std::runtime_error("Grade scale does not include 0.");
    }

    auto it = gradeScale.lower_bound(100.0f);

    if (it != gradeScale.end()) {
        throw std::runtime_error("Grade scale includes values greater than 100.");
    }
}

// iterates over all assignments in the Course, groups completed assignments by category, and computes
// raw percentage grade for each category; the per-category averages will be weighted for the total 
// grade calculation
void Course::calculateGradesByCategory() {
    std::unordered_map<std::string, float> totals;
    std::unordered_map<std::string, unsigned int> counts;

    // add values from assignmentList to totals and counts
    for (const auto& [id, assignment] : assignmentList_) {
        if (!assignment.getCompleted()) {
            continue;
        }

        const std::string& category = assignment.getCategory();
        totals[category] += assignment.getGrade();
        counts[category]++;
    }

    gradesByCategory_.clear();

    // calculate category grades and store in map
    for (const auto& [categoryName, _] : gradeWeights_) {
        auto it = counts.find(categoryName);

        if (it == counts.end() || it->second == 0) {
            continue;
        }

        float categoryGrade = totals[categoryName] / it->second;
        gradesByCategory_.emplace(categoryName, utils::floatRound(categoryGrade, 2));
    }
}

// calculate course grade percentage using grades from each category
float Course::calculateGradePct() {
    if (assignmentList_.size() == 0 || calculateCompletedAssignments() == 0) {
        return 0.0f;
    }

    float total{0.0f};
    float activeWeightTotal{0.0f};  // used to redistribute weights if 1+ categories are empty

    calculateGradesByCategory();    // call this to get category grades before weighing each piece
    // sum categories that have grades
    for (const auto& [category, weight] : gradeWeights_) {
        if (gradesByCategory_.contains(category)) {
            activeWeightTotal += weight;
        }
    }

    // return early if no active categories
    if (utils::floatEqual(activeWeightTotal, 0.0f)) {
        return 0.0f;
    }
    
    // normalize weights and calculate overall grade
    for (const auto& [categoryName, grade] : gradesByCategory_) {
        auto it = gradeWeights_.find(categoryName);
        
        float normalizedWeight = it->second / activeWeightTotal;
        float weightedGrade = grade * normalizedWeight;
        total += weightedGrade;
    }

    return utils::floatRound(total, 2);
}

// calculate letter grade based on grade percentage and given grade scale; returns "N/A" when no assignments are completed
std::string Course::calculateLetterGrade(float gradePct, const std::map<float, std::string>& gradeScale) const {
    if (utils::floatEqual(gradePct, 0.0f) && calculateCompletedAssignments() == 0) {
        // grade not determined if all assignments are incomplete
        return "N/A";
    }
    
    auto iter = gradeScale.upper_bound(gradePct);

    // if gradePct > the highest key, use the last entry
    if (iter == gradeScale.end()) {
        return gradeScale.rbegin()->second;
    }

    // otherwise, use the largest key that is <= gradePct
    --iter;

    return iter->second;
}

// calculate letter grade using the existing grade scale
std::string Course::calculateLetterGrade(float gradePct) const {
    return calculateLetterGrade(gradePct, gradeScale_);
}

// calculate GPA value based on the letter grade
float Course::calculateGpaVal(const std::string& letterGrade) {
    return gpaScale_.at(letterGrade);
}

// calculate number of completed assignments by using the boolean values of Assignment
int Course::calculateCompletedAssignments() const {
    int completedAssignments = 0;

    for (const auto& [id, assignment] : assignmentList_) {
        if (assignment.getCompleted()) {
            completedAssignments++;
        }
    }

    return completedAssignments;
}

Course::Course(std::string title, std::string description, std::chrono::year_month_day startDate, std::chrono::year_month_day endDate, 
    int numCredits, bool active) : id_{utils::generateUuid()} {
    // internal defaulting for user input
    if (startDate == std::chrono::year_month_day{}) {
        startDate = utils::defaultStartDate();
    }

    if (endDate == std::chrono::year_month_day{}) {
        endDate = utils::defaultEndDate(startDate);
    }
    
    // input validation before moving to member variables
    utils::validateReqString(title, "Title");
    utils::validateDate(startDate);
    utils::validateDate(endDate);
    utils::validateDateOrder(startDate, endDate);
    validateNumCredits(numCredits);

    title_ = std::move(title);
    if (!utils::isOnlyWhitespace(description)) {
        description_ = std::move(description);
    }
    startDate_ = startDate;
    endDate_ = endDate;
    numCredits_ = numCredits;
    active_ = active;
}

std::string Course::getId() const {
    return id_;
}

std::string Course::getTitle() const {
    return title_;
}

std::string Course::getDescription() const {
    return description_;
}

std::chrono::year_month_day Course::getStartDate() const {
    return startDate_;
}

std::chrono::year_month_day Course::getEndDate() const {
    return endDate_;
}

const std::unordered_map<std::string, Assignment>& Course::getAssignmentList() const {
    return assignmentList_;
}

const std::unordered_map<std::string, float>& Course::getGradeWeights() const {
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

std::map<float, std::string> Course::getGradeScale() const {
    return gradeScale_;
}

void Course::setTitle(std::string newTitle) {
    utils::validateReqString(newTitle, "Title");
    title_ = newTitle;
}

void Course::setDescription(std::string newDescription) {
    if (!utils::isOnlyWhitespace(newDescription)) {
        description_ = std::move(newDescription);
    } else {
        description_ = "";
    }
}

void Course::setStartDate(std::chrono::year_month_day newStartDate) {
    utils::validateDate(newStartDate);
    startDate_ = newStartDate;
}

void Course::setEndDate(std::chrono::year_month_day newEndDate) {
    utils::validateDate(newEndDate);
    endDate_ = newEndDate;
}

void Course::setGradeWeights(const std::unordered_map<std::string, float>& newGradeWeights) {
    validateGradeWeights(newGradeWeights);
    gradeWeights_ = newGradeWeights;
}

void Course::setNumCredits(int newNumCredits) {
    validateNumCredits(newNumCredits);
    numCredits_ = newNumCredits;
}

// sets grade percentage automatically through calculation
void Course::setGradePct() {
    gradePct_ = calculateGradePct();
}

// sets grade percentage manually through user input
void Course::setGradePct(float newGradePct) {
    validateGradePct(newGradePct);
    gradePct_ = newGradePct;
}

void Course::setLetterGrade() {
    letterGrade_ = calculateLetterGrade(gradePct_);
}

void Course::setGpaVal() {
    if (letterGrade_ == "N/A") {
        setLetterGrade();
    }
    
    gpaVal_ = calculateGpaVal(letterGrade_);
}

void Course::setActive(bool newActive) {
    active_ = newActive;
}

void Course::setGradeScale(const std::map<float, std::string>& newGradeScale) {
    validateGradeScale(newGradeScale);
    gradeScale_ = newGradeScale;
}

// prints information held by a Course object
void Course::printCourseInfo(std::ostream &os) const {
    os << "ID: " << id_ << "\n";
    os << "Course: " << title_ << "\n";
    if (!description_.empty()) {
        os << "Description: " << description_ << "\n";
    }
    os << "Duration: " << startDate_ << " - " << endDate_ << "\n";
    os << "Number of Credits: " << numCredits_ << "\n";
    os << "Grade Percentage: " << std::fixed << std::setprecision(2) << gradePct_ << "%\n";
    os << "Letter Grade: " << letterGrade_ << "\n";
    os << "GPA Value: " << std::fixed << std::setprecision(1) << gpaVal_ << "\n";
    os << "Total Assignments: " << assignmentList_.size() << "\n";
    os << "Incomplete Assignments: " << assignmentList_.size() - calculateCompletedAssignments() << "\n";
    os << "Current? " << utils::boolToString(active_) << "\n";
}

// adds an Assignment to the end of the list from the given input
void Course::addAssignment(const Assignment &assignment) {
    auto [_, inserted] = assignmentList_.emplace(assignment.getId(), assignment);

    if (!inserted) {
        throw std::logic_error("Assignment with the same ID already exists.");
    }

    // update grade information
    setGradePct();
    setLetterGrade();
    setGpaVal();
}

// removes an Assignment with the specified UUID
void Course::removeAssignment(const std::string& id) {
    if (assignmentList_.erase(id) == 0) {
        throw std::out_of_range("Assignment not found.");
    }

    // update grade information
    setGradePct();
    setLetterGrade();
    setGpaVal();
}

// finds an Assignment in assignmentList based on ID; non-mutable (read-only)
const Assignment& Course::findAssignment(const std::string& id) const {
    auto it = assignmentList_.find(id);

    if (it != assignmentList_.end()) {
        return it->second;
    } else {
        throw std::out_of_range("Assignment not found.");
    }
}

// finds an Assignment in assignmentList based on ID; mutable (read and write access)
Assignment& Course::findAssignment(const std::string& id) {
    // use const casting to use the same logic as the const version without duplication
    const Course &selfConst = *this;
    return const_cast<Assignment&>(selfConst.findAssignment(id));
}

// equality comparison based on unique identifier (UUID)
bool Course::operator==(const Course &other) const {
    return id_ == other.id_;
}