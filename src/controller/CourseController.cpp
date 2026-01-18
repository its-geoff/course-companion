#include "controller/CourseController.hpp"

/**
 * @file CourseController.cpp
 * @brief Implementation of a controller that manages interaction between the views and Course.
 * 
 * This controller implements functions that integrate the classes together and allow for code 
 * reusability within the main function.
 */

#include <exception>            // for exceptions
#include "utils/utils.hpp"      // for stringLower

CourseController::CourseController(Term& term) : term_{term} {}

const std::unordered_map<std::string, Course>& CourseController::getCourseList() const {
    return term_.getCourseList();
}

std::string CourseController::getCourseId(const std::string &title) const {
    std::string titleLower = utils::stringLower(title);
    auto it = titleToId_.find(titleLower);

    if (it == titleToId_.end()) {
        throw std::out_of_range("Course not found.");
    }

    return it->second;
}

AssignmentController& CourseController::getAssignmentController() {
    if (!assignmentController_.has_value()) {
        throw std::logic_error("No course selected.");
    }

    return *assignmentController_;
}

// uses info from view to create a Course object then adds it to the list of Courses in Term
void CourseController::addCourse(const std::string& title, const std::string& description, const std::chrono::year_month_day& startDate,
    const std::chrono::year_month_day& endDate, int numCredits, bool active) {
    Course course{title, description, startDate, endDate, numCredits, active};

    try {
        term_.addCourse(course);
    } catch (const std::exception& e) {
        throw std::runtime_error("An unexpected error occurred when adding the course.");
    }

    // make title lowercase in titleToId for easier comparison
    auto [_, inserted] = titleToId_.emplace(utils::stringLower(course.getTitle()), course.getId());

    if (!inserted) {
        term_.removeCourse(course.getId());     // erase Course object if there's an error
        throw std::logic_error("Course with the same title already exists.");
    }
}

// edits the title of the Course with the given ID
void CourseController::editTitle(const std::string& id, const std::string& newTitle) {
    Course& course = term_.findCourse(id);
    std::string oldTitle = course.getTitle();

    auto [_, inserted] = titleToId_.emplace(utils::stringLower(newTitle), id);

    if (!inserted) {
        throw std::logic_error("A course with this title already exists.");
    }

    // change the title -> id mapping for the new title
    titleToId_.erase(utils::stringLower(oldTitle));

    // set title after insertion to title -> id mapping
    course.setTitle(newTitle);
}

// edits the description of the Course with the given ID
void CourseController::editDescription(const std::string& id, const std::string& newDescription) {
    Course& course = term_.findCourse(id);
    course.setDescription(newDescription);
}

// edits the start date of the Course with the given ID
void CourseController::editStartDate(const std::string& id, const std::chrono::year_month_day& newStartDate) {
    Course& course = term_.findCourse(id);
    course.setStartDate(newStartDate);
}

// edits the end date of the Course with the given ID
void CourseController::editEndDate(const std::string& id, const std::chrono::year_month_day& newEndDate) {
    Course& course = term_.findCourse(id);
    course.setEndDate(newEndDate);
}

// edits the number of credits of the Course with the given ID
void CourseController::editNumCredits(const std::string& id, int newNumCredits) {
    Course& course = term_.findCourse(id);
    course.setNumCredits(newNumCredits);
}

// edits the active flag of the Course with the given ID
void CourseController::editActive(const std::string& id, bool newActive) {
    Course& course = term_.findCourse(id);
    course.setActive(newActive);
}

// searches title -> id and erases the named Course from the list
void CourseController::removeCourse(const std::string& title) {
    std::string id = getCourseId(title);
    term_.removeCourse(id);
    titleToId_.erase(utils::stringLower(title));
}

// find a Course in courseList based on title; non-mutable (read-only)
const Course& CourseController::findCourse(const std::string& title) const {
    std::string id = getCourseId(title);
    return term_.findCourse(id);
}

// find a Course in courseList based on title; mutable (read and write access)
Course& CourseController::findCourse(const std::string& title) {
    std::string id = getCourseId(title);
    return term_.findCourse(id);
}

// selects a Course and makes it "active", creating an AssignmentController for that course
void CourseController::selectCourse(const std::string& title) {
    std::string id = getCourseId(title);
    
    try {
        Course& courseRef = term_.findCourse(id);
        activeCourse_ = &courseRef;
        assignmentController_.emplace(*activeCourse_);
    } catch (const std::exception& e) {
        throw std::out_of_range("Course not found.");
    }
}