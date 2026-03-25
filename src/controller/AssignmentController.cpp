#include "controller/AssignmentController.hpp"

/**
 * @file AssignmentController.cpp
 * @brief Implementation of a controller that manages interaction between the views and Assignment.
 * 
 * This controller implements functions that integrate the classes together and allow for code 
 * reusability within the main function.
 */

#include <exception>            // for exceptions
#include "utils/utils.hpp"      // for stringLower

AssignmentController::AssignmentController(Course& course, DatabaseConnection& db)
    : course_{course}, db_{db}, assignmentRepo_{db, course.getId()} {}

const std::unordered_map<std::string, Assignment>& AssignmentController::getAssignmentList() const {
    return course_.getAssignmentList();
}

std::string AssignmentController::getAssignmentId(const std::string& title) const {
    std::string titleLower = utils::stringLower(title);
    auto it = titleToId_.find(titleLower);

    if (it == titleToId_.end()) {
        throw std::out_of_range("Assignment not found.");
    }

    return it->second;
}

// loads all assignments for the current course from the database into course_ on startup
void AssignmentController::loadFromDb() {
    std::vector<Assignment> assignments = assignmentRepo_.findByParentId(course_.getId());

    for (Assignment& assignment : assignments) {
        titleToId_.emplace(utils::stringLower(assignment.getTitle()), assignment.getId());
        course_.addAssignment(assignment);
    }
}

// uses info from view to create an Assignment object, adds it to the course, and persists it to the database
void AssignmentController::addAssignment(const std::string& title, const std::string& description, const std::string& category,
    const std::chrono::year_month_day& dueDate, bool completed, float grade) {
    Assignment assignment{title, description, category, dueDate, completed, grade};

    if (!course_.getGradeWeights().contains(category)) {
        throw std::out_of_range("Invalid category. Category must be in grade weights.");
    }

    try {
        course_.addAssignment(assignment);
    } catch (const std::exception& e) {
        throw std::runtime_error("An unexpected error occurred when adding the assignment.");
    }

    auto [_, inserted] = titleToId_.emplace(utils::stringLower(assignment.getTitle()), assignment.getId());

    if (!inserted) {
        course_.removeAssignment(assignment.getId());
        throw std::logic_error("Assignment with the same title already exists.");
    }

    assignmentRepo_.insert(assignment);
}

// edits the title of the Assignment with the given ID and syncs to the database
void AssignmentController::editTitle(const std::string& id, const std::string& newTitle) {
    Assignment& assignment = course_.findAssignment(id);
    std::string oldTitle = assignment.getTitle();

    auto [_, inserted] = titleToId_.emplace(utils::stringLower(newTitle), id);

    if (!inserted) {
        throw std::logic_error("An assignment with this title already exists.");
    }

    titleToId_.erase(utils::stringLower(oldTitle));
    assignment.setTitle(newTitle);
    assignmentRepo_.update(assignment);
}

// edits the description of the Assignment with the given ID and syncs to the database
void AssignmentController::editDescription(const std::string& id, const std::string& newDescription) {
    Assignment& assignment = course_.findAssignment(id);
    assignment.setDescription(newDescription);
    assignmentRepo_.update(assignment);
}

// edits the category of the Assignment with the given ID and syncs to the database
void AssignmentController::editCategory(const std::string& id, const std::string& newCategory) {
    Assignment& assignment = course_.findAssignment(id);

    if (utils::isOnlyWhitespace(newCategory)) {
        throw std::invalid_argument("Category must be non-empty.");
    }

    if (!course_.getGradeWeights().contains(newCategory)) {
        throw std::out_of_range("Invalid category. Category must be in grade weights.");
    }

    assignment.setCategory(newCategory);
    assignmentRepo_.update(assignment);
}

// edits the due date of the Assignment with the given ID and syncs to the database
void AssignmentController::editDueDate(const std::string& id, const std::chrono::year_month_day& newDueDate) {
    Assignment& assignment = course_.findAssignment(id);
    assignment.setDueDate(newDueDate);
    assignmentRepo_.update(assignment);
}

// adds a grade to an incomplete assignment, sets it to completed, and syncs to the database; percentage-based overload
void AssignmentController::addGrade(const std::string& title, float grade) {
    std::string id = getAssignmentId(title);
    Assignment& selectedAssignment = course_.findAssignment(id);

    grade = utils::floatRound(grade, 2);
    selectedAssignment.setGrade(grade);
    selectedAssignment.setCompleted(true);
    assignmentRepo_.update(selectedAssignment);
}

// adds a grade to an incomplete assignment, sets it to completed, and syncs to the database; point-based overload
void AssignmentController::addGrade(const std::string& title, float pointsEarned, float totalPoints) {
    if (utils::floatEqual(totalPoints, 0.0f)) {
        throw std::invalid_argument("Division by zero not allowed.");
    }

    float calculatedGrade = (pointsEarned / totalPoints) * 100.0f;
    addGrade(title, calculatedGrade);
}

// removes a grade from a completed assignment, resets grade and completion status, and syncs to the database
void AssignmentController::removeGrade(const std::string& title) {
    std::string id = getAssignmentId(title);
    Assignment& selectedAssignment = course_.findAssignment(id);

    selectedAssignment.setGrade(0.0f);
    selectedAssignment.setCompleted(false);
    assignmentRepo_.update(selectedAssignment);
}

// searches title -> id, erases the named Assignment from the course, and removes it from the database
void AssignmentController::removeAssignment(const std::string& title) {
    std::string id = getAssignmentId(title);
    assignmentRepo_.remove(id);
    course_.removeAssignment(id);
    titleToId_.erase(utils::stringLower(title));
}

// finds an Assignment in assignmentList based on title; non-mutable (read-only)
const Assignment& AssignmentController::findAssignment(const std::string& title) const {
    std::string id = getAssignmentId(title);
    return course_.findAssignment(id);
}

// finds an Assignment in assignmentList based on title; mutable (read and write access)
Assignment& AssignmentController::findAssignment(const std::string& title) {
    std::string id = getAssignmentId(title);
    return course_.findAssignment(id);
}