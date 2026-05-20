#ifndef ASSIGNMENTCONTROLLER_HPP
#define ASSIGNMENTCONTROLLER_HPP

#include <string>
#include <chrono>
#include <unordered_map>
#include <memory>
#include "model/Course.hpp"
#include "model/Assignment.hpp"
#include "db/DatabaseConnection.hpp"
#include "db/AssignmentRepository.hpp"

class AssignmentController {
    private:
        Course& course_;
        std::unordered_map<std::string, std::string> titleToId_{};
        DatabaseConnection* db_ = nullptr;
        std::unique_ptr<AssignmentRepository> assignmentRepo_;

    public:
        AssignmentController(Course& course);               // no-db constructor for unit tests
        AssignmentController(Course& course, DatabaseConnection& db);
        AssignmentController(const AssignmentController&) = delete;
        AssignmentController& operator=(const AssignmentController&) = delete;
        AssignmentController(AssignmentController&&) = delete;
        AssignmentController& operator=(AssignmentController&&) = delete;

        const std::unordered_map<std::string, Assignment>& getAssignmentList() const;
        std::string getAssignmentId(const std::string& title) const;

        void loadFromDb();
        void addAssignment(const std::string& title, const std::string& description, const std::string& category,
            const std::chrono::year_month_day& dueDate, bool completed, float grade);
        void editTitle(const std::string& id, const std::string& newTitle);
        void editDescription(const std::string& id, const std::string& newDescription);
        void editCategory(const std::string& id, const std::string& newCategory);
        void editDueDate(const std::string& id, const std::chrono::year_month_day& newDueDate);
        void addGrade(const std::string& title, float grade);
        void addGrade(const std::string& title, float pointsEarned, float totalPoints);
        void removeGrade(const std::string& title);
        void removeAssignment(const std::string& title);
        const Assignment& findAssignment(const std::string& title) const;
        Assignment& findAssignment(const std::string& title);
};

#endif