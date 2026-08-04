#ifndef ASSIGNMENTCONTROLLER_HPP
#define ASSIGNMENTCONTROLLER_HPP

/**
 * @file AssignmentController.hpp
 * @brief Definition of a controller that manages interaction between Assignment and the views.
 * 
 * This controller defines functions that integrate the Assignment class with the views and allow for the 
 * model to connect to the main function.
 */

#include <string>
#include <chrono>
#include <QObject>
#include <unordered_map>
#include "model/Course.hpp"
#include "model/Assignment.hpp"

class AssignmentController : public QObject {
    Q_OBJECT

    public:
        explicit AssignmentController(Course& course);
        AssignmentController(const AssignmentController&) = delete;
        AssignmentController& operator=(const AssignmentController&) = delete;
        AssignmentController(AssignmentController&&) = delete;
        AssignmentController& operator=(AssignmentController&&) = delete;

        const std::unordered_map<std::string, Assignment>& getAssignmentList() const;
        std::string getAssignmentId(const std::string& title) const;

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

    signals:
        void dataChanged();

    private:
        Course& course_;
        std::unordered_map<std::string, std::string> titleToId_{};
};

#endif  // ASSIGNMENTCONTROLLER_HPP