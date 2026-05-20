#include "controller/AssignmentController.hpp"

#include <exception>
#include "utils/utils.hpp"

AssignmentController::AssignmentController(Course& course)
    : course_{course}, db_{nullptr}, assignmentRepo_{nullptr} {}

AssignmentController::AssignmentController(Course& course, DatabaseConnection& db)
    : course_{course}, db_{&db}, assignmentRepo_{std::make_unique<AssignmentRepository>(db, course.getId())} {}

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

void AssignmentController::loadFromDb() {
    if (!assignmentRepo_) {
        throw std::logic_error("AssignmentController::loadFromDb() requires a database-backed repository.");
    }

    std::vector<Assignment> assignments = assignmentRepo_->findByParentId(course_.getId());

    for (Assignment& assignment : assignments) {
        titleToId_.emplace(utils::stringLower(assignment.getTitle()), assignment.getId());
        course_.addAssignment(assignment);
    }
}

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

    auto inserted = titleToId_.emplace(utils::stringLower(assignment.getTitle()), assignment.getId()).second;

    if (!inserted) {
        course_.removeAssignment(assignment.getId());
        throw std::logic_error("Assignment with the same title already exists.");
    }

    if (assignmentRepo_) {
        try {
            assignmentRepo_->insert(assignment);
        } catch (const std::exception& e) {
            // roll back previous state if saving fails
            titleToId_.erase(utils::stringLower(assignment.getTitle()));
            course_.removeAssignment(assignment.getId());
            throw std::runtime_error("Failed to save assignment.");
        }
    }
}

void AssignmentController::editTitle(const std::string& id, const std::string& newTitle) {
    Assignment& assignment = course_.findAssignment(id);
    std::string oldTitle = assignment.getTitle();

    auto inserted = titleToId_.emplace(utils::stringLower(newTitle), id).second;

    if (!inserted) {
        throw std::logic_error("An assignment with this title already exists.");
    }

    titleToId_.erase(utils::stringLower(oldTitle));
    assignment.setTitle(newTitle);

    if (assignmentRepo_) {
        assignmentRepo_->update(assignment);
    }
}

void AssignmentController::editDescription(const std::string& id, const std::string& newDescription) {
    Assignment& assignment = course_.findAssignment(id);
    assignment.setDescription(newDescription);

    if (assignmentRepo_) {
        assignmentRepo_->update(assignment);
    }
}

void AssignmentController::editCategory(const std::string& id, const std::string& newCategory) {
    Assignment& assignment = course_.findAssignment(id);

    if (utils::isOnlyWhitespace(newCategory)) {
        throw std::invalid_argument("Category must be non-empty.");
    }

    if (!course_.getGradeWeights().contains(newCategory)) {
        throw std::out_of_range("Invalid category. Category must be in grade weights.");
    }

    assignment.setCategory(newCategory);

    if (assignmentRepo_) {
        assignmentRepo_->update(assignment);
    }
}

void AssignmentController::editDueDate(const std::string& id, const std::chrono::year_month_day& newDueDate) {
    Assignment& assignment = course_.findAssignment(id);
    assignment.setDueDate(newDueDate);

    if (assignmentRepo_) {
        assignmentRepo_->update(assignment);
    }
}

void AssignmentController::addGrade(const std::string& title, float grade) {
    std::string id = getAssignmentId(title);
    Assignment& selectedAssignment = course_.findAssignment(id);

    grade = utils::floatRound(grade, 2);
    selectedAssignment.setGrade(grade);
    selectedAssignment.setCompleted(true);

    if (assignmentRepo_) {
        assignmentRepo_->update(selectedAssignment);
    }
}

void AssignmentController::addGrade(const std::string& title, float pointsEarned, float totalPoints) {
    if (utils::floatEqual(totalPoints, 0.0f)) {
        throw std::invalid_argument("Division by zero not allowed.");
    }

    float calculatedGrade = (pointsEarned / totalPoints) * 100.0f;
    addGrade(title, calculatedGrade);
}

void AssignmentController::removeGrade(const std::string& title) {
    std::string id = getAssignmentId(title);
    Assignment& selectedAssignment = course_.findAssignment(id);

    selectedAssignment.setGrade(0.0f);
    selectedAssignment.setCompleted(false);

    if (assignmentRepo_) {
        assignmentRepo_->update(selectedAssignment);
    }
}

void AssignmentController::removeAssignment(const std::string& title) {
    std::string id = getAssignmentId(title);

    if (assignmentRepo_) {
        assignmentRepo_->remove(id);
    }

    course_.removeAssignment(id);
    titleToId_.erase(utils::stringLower(title));
}

const Assignment& AssignmentController::findAssignment(const std::string& title) const {
    std::string id = getAssignmentId(title);
    return course_.findAssignment(id);
}

Assignment& AssignmentController::findAssignment(const std::string& title) {
    std::string id = getAssignmentId(title);
    return course_.findAssignment(id);
}