#ifndef COURSECONTROLLER_HPP
#define COURSECONTROLLER_HPP

/**
 * @file CourseController.hpp
 * @brief Definition of a controller that manages interaction between Course and the views.
 * 
 * This controller defines functions that integrate the Course class with the views and allow for the 
 * model to connect to the main function.
 */

#include <string>
#include <chrono>
#include <unordered_map>
#include <optional>
#include <QObject>
#include "model/Term.hpp"
#include "model/Course.hpp"
#include "controller/AssignmentController.hpp"

class CourseController : public QObject {
    Q_OBJECT

    public:
        explicit CourseController(Term& term);
        CourseController(const CourseController&) = delete;
        CourseController& operator=(const CourseController&) = delete;
        CourseController(CourseController&&) = delete;
        CourseController& operator=(CourseController&&) = delete;

        const std::unordered_map<std::string, Course>& getCourseList() const;
        std::string getCourseId(const std::string& title) const;
        AssignmentController& getAssignmentController();

        void addCourse(const std::string& title, const std::string& description, const std::chrono::year_month_day& startDate,
            const std::chrono::year_month_day& endDate, int numCredits, bool active);
        void editTitle(const std::string& id, const std::string& newTitle);
        void editDescription(const std::string& id, const std::string& newDescription);
        void editStartDate(const std::string& id, const std::chrono::year_month_day& newStartDate);
        void editEndDate(const std::string& id, const std::chrono::year_month_day& newEndDate);
        void editNumCredits(const std::string& id, int newNumCredits);
        void editActive(const std::string& id, bool newActive);
        void removeCourse(const std::string& title);
        const Course& findCourse(const std::string& title) const;
        Course& findCourse(const std::string& title);
        void selectCourse(const std::string& title);

    signals:
        void dataChanged();
        void courseSelected();

    private:
        Term& term_;
        Course* activeCourse_ = nullptr;
        std::unordered_map<std::string, std::string> titleToId_{};
        std::optional<AssignmentController> assignmentController_{};
};

#endif  // COURSECONTROLLER_HPP