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

AssignmentController::AssignmentController(Course& course) : course_{course} {}

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

// uses info from view to create an Assignment object then adds it to the list of Assignments in Assignment
void AssignmentController::addAssignment(const std::string& title, const std::string& description, 
    const std::chrono::year_month_day& dueDate, bool completed, float grade) {
    Assignment assignment{title, description, dueDate, completed, grade};

    try {
        course_.addAssignment(assignment);
    } catch (const std::exception& e) {
        throw std::runtime_error("An unexpected error occurred when adding the assignment.");
    }

    // make title lowercase in titleToId for easier comparison
    auto [_, inserted] = titleToId_.emplace(utils::stringLower(assignment.getTitle()), assignment.getId());

    if (!inserted) {
        course_.removeAssignment(assignment.getId());   // erase Assignment object if there's an error
        throw std::logic_error("Assignment with the same title already exists.");
    }
}

// edits the title of the Assignment with the given ID
void AssignmentController::editTitle(const std::string& id, const std::string& newTitle) {
    Assignment& assignment = course_.findAssignment(id);
    std::string oldTitle = assignment.getTitle();

    auto [_, inserted] = titleToId_.emplace(utils::stringLower(newTitle), id);

    if (!inserted) {
        throw std::logic_error("An assignment with this title already exists.");
    }

    // change the title -> id mapping for the new title
    titleToId_.erase(utils::stringLower(oldTitle));

    // set title after insertion to title -> id mapping
    assignment.setTitle(newTitle);
}

// edits the description of the Assignment with the given ID
void AssignmentController::editDescription(const std::string& id, const std::string& newDescription) {
    Assignment& assignment = course_.findAssignment(id);
    assignment.setDescription(newDescription);
}

// edits the due date of the Assignment with the given ID
void AssignmentController::editDueDate(const std::string& id, const std::chrono::year_month_day& newDueDate) {
    Assignment& assignment = course_.findAssignment(id);
    assignment.setDueDate(newDueDate);
}

// edits the completed flag of the Assignment with the given ID
void AssignmentController::editCompleted(const std::string& id, bool newCompleted) {
    Assignment& assignment = course_.findAssignment(id);
    assignment.setCompleted(newCompleted);
}

// edits the grade of the Assignment with the given ID
void AssignmentController::editGrade(const std::string& id, float newGrade) {
    Assignment& assignment = course_.findAssignment(id);
    assignment.setGrade(newGrade);
}

// searches title -> id and erases the named Assignment from the list
void AssignmentController::removeAssignment(const std::string& title) {
    std::string id = getAssignmentId(title);
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

// selects an Assignment and makes it "active"
void AssignmentController::selectAssignment(const std::string& title) {
    std::string id = getAssignmentId(title);

    try {
        Assignment& assignmentRef = course_.findAssignment(id);
        activeAssignment_ = &assignmentRef;
    } catch (const std::exception& e) {
        throw std::out_of_range("Assignment not found");
    }
}