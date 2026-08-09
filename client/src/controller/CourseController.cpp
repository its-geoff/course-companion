#include "controller/CourseController.hpp"

#include <exception>
#include <algorithm>
#include "utils/utils.hpp"

CourseController::CourseController(Term& term) : term_{term} {}

const std::unordered_map<std::string, Course>& CourseController::getCourseList() const {
    return term_.getCourseList();
}

const std::vector<std::string>& CourseController::getCourseOrder() const {
    return courseOrder_;
}

std::string CourseController::getCourseId(const std::string& title) const {
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

void CourseController::addCourse(const std::string& title, const std::string& description, const std::chrono::year_month_day& startDate,
    const std::chrono::year_month_day& endDate, int numCredits, bool active) {
    Course course{title, description, startDate, endDate, numCredits, active};

    try {
        term_.addCourse(course);
    } catch (const std::exception& e) {
        throw std::runtime_error("An unexpected error occurred when adding the course.");
    }

    auto inserted = titleToId_.emplace(utils::stringLower(course.getTitle()), course.getId()).second;

    if (!inserted) {
        term_.removeCourse(course.getId());
        throw std::logic_error("Course with the same title already exists.");
    }

    courseOrder_.push_back(course.getId());
    emit dataChanged();
}

void CourseController::editTitle(const std::string& id, const std::string& newTitle) {
    Course& course = term_.findCourse(id);
    std::string oldTitle = course.getTitle();

    if (titleToId_.contains(utils::stringLower(newTitle))) {
        throw std::logic_error("A course with this title already exists.");
    }

    course.setTitle(newTitle);
    titleToId_.erase(utils::stringLower(oldTitle));
    titleToId_.emplace(utils::stringLower(newTitle), id);
    emit dataChanged();
}

void CourseController::editDescription(const std::string& id, const std::string& newDescription) {
    Course& course = term_.findCourse(id);
    course.setDescription(newDescription);
    emit dataChanged();
}

void CourseController::editStartDate(const std::string& id, const std::chrono::year_month_day& newStartDate) {
    Course& course = term_.findCourse(id);
    course.setStartDate(newStartDate);
    emit dataChanged();
}

void CourseController::editEndDate(const std::string& id, const std::chrono::year_month_day& newEndDate) {
    Course& course = term_.findCourse(id);
    course.setEndDate(newEndDate);
    emit dataChanged();
}

void CourseController::editNumCredits(const std::string& id, int newNumCredits) {
    Course& course = term_.findCourse(id);
    course.setNumCredits(newNumCredits);
    emit dataChanged();
}

void CourseController::editActive(const std::string& id, bool newActive) {
    Course& course = term_.findCourse(id);
    course.setActive(newActive);
    emit dataChanged();
}

void CourseController::removeCourse(const std::string& title) {
    const std::string id = getCourseId(title);

    if (activeCourse_ != nullptr && activeCourse_->getId() == id) {
        activeCourse_ = nullptr;
        assignmentController_.reset();
    }

    term_.removeCourse(id);
    titleToId_.erase(utils::stringLower(title));
    courseOrder_.erase(std::remove(courseOrder_.begin(), courseOrder_.end(), id), courseOrder_.end());
    emit dataChanged();
}

const Course& CourseController::findCourse(const std::string& title) const {
    std::string id = getCourseId(title);
    return term_.findCourse(id);
}

Course& CourseController::findCourse(const std::string& title) {
    std::string id = getCourseId(title);
    return term_.findCourse(id);
}

void CourseController::selectCourse(const std::string& title) {
    std::string id = getCourseId(title);

    try {
        Course& courseRef = term_.findCourse(id);
        activeCourse_ = &courseRef;
        assignmentController_.emplace(*activeCourse_);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Course not found.");
    }
    emit courseSelected();
}