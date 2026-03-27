#include "controller/CourseController.hpp"

#include <exception>
#include "utils/utils.hpp"

CourseController::CourseController(Term& term)
    : term_{term}, db_{nullptr}, courseRepo_{nullptr} {}

CourseController::CourseController(Term& term, DatabaseConnection& db)
    : term_{term}, db_{&db}, courseRepo_{std::make_unique<CourseRepository>(db, term.getId())} {}

const std::unordered_map<std::string, Course>& CourseController::getCourseList() const {
    return term_.getCourseList();
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

void CourseController::loadFromDb() {
    std::vector<Course> courses = courseRepo_->findByParentId(term_.getId());

    for (Course& course : courses) {
        titleToId_.emplace(utils::stringLower(course.getTitle()), course.getId());
        term_.addCourse(course);
    }
}

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

    if (courseRepo_) {
        courseRepo_->insert(course);
    }
}

void CourseController::editTitle(const std::string& id, const std::string& newTitle) {
    Course& course = term_.findCourse(id);
    std::string oldTitle = course.getTitle();

    auto [_, inserted] = titleToId_.emplace(utils::stringLower(newTitle), id);

    if (!inserted) {
        throw std::logic_error("A course with this title already exists.");
    }

    titleToId_.erase(utils::stringLower(oldTitle));
    course.setTitle(newTitle);

    if (courseRepo_) {
        courseRepo_->update(course);
    }
}

void CourseController::editDescription(const std::string& id, const std::string& newDescription) {
    Course& course = term_.findCourse(id);
    course.setDescription(newDescription);

    if (courseRepo_) {
        courseRepo_->update(course);
    }
}

void CourseController::editStartDate(const std::string& id, const std::chrono::year_month_day& newStartDate) {
    Course& course = term_.findCourse(id);
    course.setStartDate(newStartDate);

    if (courseRepo_) {
        courseRepo_->update(course);
    }
}

void CourseController::editEndDate(const std::string& id, const std::chrono::year_month_day& newEndDate) {
    Course& course = term_.findCourse(id);
    course.setEndDate(newEndDate);

    if (courseRepo_) {
        courseRepo_->update(course);
    }
}

void CourseController::editNumCredits(const std::string& id, int newNumCredits) {
    Course& course = term_.findCourse(id);
    course.setNumCredits(newNumCredits);

    if (courseRepo_) {
        courseRepo_->update(course);
    }
}

void CourseController::editActive(const std::string& id, bool newActive) {
    Course& course = term_.findCourse(id);
    course.setActive(newActive);

    if (courseRepo_) {
        courseRepo_->update(course);
    }
}

void CourseController::removeCourse(const std::string& title) {
    std::string id = getCourseId(title);

    if (courseRepo_) {
        courseRepo_->remove(id);
    }

    term_.removeCourse(id);
    titleToId_.erase(utils::stringLower(title));
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
        if (db_) {
            assignmentController_.emplace(*activeCourse_, *db_);
        } else {
            assignmentController_.emplace(*activeCourse_);
        }
    } catch (const std::exception& e) {
        throw std::out_of_range("Course not found.");
    }
}