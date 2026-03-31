#ifndef COURSECONTROLLER_HPP
#define COURSECONTROLLER_HPP

#include <string>
#include <chrono>
#include <unordered_map>
#include <optional>
#include <memory>
#include "model/Term.hpp"
#include "controller/AssignmentController.hpp"
#include "db/DatabaseConnection.hpp"
#include "db/CourseRepository.hpp"
#include "model/Course.hpp"

class CourseController {
    private:
        Term& term_;
        Course* activeCourse_ = nullptr;
        std::unordered_map<std::string, std::string> titleToId_{};
        std::optional<AssignmentController> assignmentController_{};
        DatabaseConnection* db_ = nullptr;
        std::unique_ptr<CourseRepository> courseRepo_;

    public:
        CourseController(Term& term);                       // no-db constructor for unit tests
        CourseController(Term& term, DatabaseConnection& db);
        CourseController(const CourseController&) = delete;
        CourseController& operator=(const CourseController&) = delete;
        CourseController(CourseController&&) = delete;
        CourseController& operator=(CourseController&&) = delete;

        const std::unordered_map<std::string, Course>& getCourseList() const;
        std::string getCourseId(const std::string& title) const;
        AssignmentController& getAssignmentController();

        void loadFromDb();
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
};

#endif