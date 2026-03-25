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

CourseController::CourseController(Term& term, DatabaseConnection& db)
    : term_{term}, db_{db}, courseRepo_{db, term.getId()} {}

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

// loads all courses for the current term from the database into term_ on startup
void CourseController::loadFromDb() {
    std::vector<Course> courses = courseRepo_.findByParentId(term_.getId());

    for (Course& course : courses) {
        titleToId_.emplace(utils::stringLower(course.getTitle()), course.getId());
        term_.addCourse(course);
    }
}

// uses info from view to create a Course object, adds it to the term, and persists it to the database
void CourseController::addCourse(const std::string& title, const std::string& description, const std::chrono::year_month_day& startDate,
    const std::chrono::year_month_day& endDate, int numCredits, bool active) {
    Course course{title, description, startDate, endDate, numCredits, active};

    try {
        term_.addCourse(course);
    } catch (const std::exception& e) {
        throw std::runtime_error("An unexpected error occurred when adding the course.");
    }

    auto [_, inserted] = titleToId_.emplace(utils::stringLower(course.getTitle()), course.getId());

    if (!inserted) {
        term_.removeCourse(course.getId());
        throw std::logic_error("Course with the same title already exists.");
    }

    courseRepo_.insert(course);
}

// edits the title of the Course with the given ID and syncs to the database
void CourseController::editTitle(const std::string& id, const std::string& newTitle) {
    Course& course = term_.findCourse(id);
    std::string oldTitle = course.getTitle();

    auto [_, inserted] = titleToId_.emplace(utils::stringLower(newTitle), id);

    if (!inserted) {
        throw std::logic_error("A course with this title already exists.");
    }

    titleToId_.erase(utils::stringLower(oldTitle));
    course.setTitle(newTitle);
    courseRepo_.update(course);
}

// edits the description of the Course with the given ID and syncs to the database
void CourseController::editDescription(const std::string& id, const std::string& newDescription) {
    Course& course = term_.findCourse(id);
    course.setDescription(newDescription);
    courseRepo_.update(course);
}

// edits the start date of the Course with the given ID and syncs to the database
void CourseController::editStartDate(const std::string& id, const std::chrono::year_month_day& newStartDate) {
    Course& course = term_.findCourse(id);
    course.setStartDate(newStartDate);
    courseRepo_.update(course);
}

// edits the end date of the Course with the given ID and syncs to the database
void CourseController::editEndDate(const std::string& id, const std::chrono::year_month_day& newEndDate) {
    Course& course = term_.findCourse(id);
    course.setEndDate(newEndDate);
    courseRepo_.update(course);
}

// edits the number of credits of the Course with the given ID and syncs to the database
void CourseController::editNumCredits(const std::string& id, int newNumCredits) {
    Course& course = term_.findCourse(id);
    course.setNumCredits(newNumCredits);
    courseRepo_.update(course);
}

// edits the active flag of the Course with the given ID and syncs to the database
void CourseController::editActive(const std::string& id, bool newActive) {
    Course& course = term_.findCourse(id);
    course.setActive(newActive);
    courseRepo_.update(course);
}

// searches title -> id, erases the named Course from the term, and removes it from the database
void CourseController::removeCourse(const std::string& title) {
    std::string id = getCourseId(title);
    courseRepo_.remove(id);
    term_.removeCourse(id);
    titleToId_.erase(utils::stringLower(title));
}

// finds a Course in courseList based on title; non-mutable (read-only)
const Course& CourseController::findCourse(const std::string& title) const {
    std::string id = getCourseId(title);
    return term_.findCourse(id);
}

// finds a Course in courseList based on title; mutable (read and write access)
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
        assignmentController_.emplace(*activeCourse_, db_);
    } catch (const std::exception& e) {
        throw std::out_of_range("Course not found.");
    }
}