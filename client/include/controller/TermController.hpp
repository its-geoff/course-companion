#ifndef TERMCONTROLLER_HPP
#define TERMCONTROLLER_HPP

#include <string>
#include <chrono>
#include <unordered_map>
#include <optional>
#include "model/Term.hpp"
#include "controller/CourseController.hpp"

class TermController {
    private:
        std::unordered_map<std::string, Term> termList_{};
        std::unordered_map<std::string, std::string> titleToId_{};
        Term* activeTerm_ = nullptr;
        std::optional<CourseController> courseController_{};

    public:
        TermController() = default;
        TermController(const TermController&) = delete;
        TermController& operator=(const TermController&) = delete;

        const std::unordered_map<std::string, Term>& getTermList() const;
        std::string getTermId(const std::string& title) const;
        CourseController& getCourseController();

        void addTerm(const std::string& title, const std::chrono::year_month_day& startDate,
            const std::chrono::year_month_day& endDate, bool active);
        void editTitle(const std::string& id, const std::string& newTitle);
        void editStartDate(const std::string& id, const std::chrono::year_month_day& newStartDate);
        void editEndDate(const std::string& id, const std::chrono::year_month_day& newEndDate);
        void editActive(const std::string& id, bool newActive);
        void removeTerm(const std::string& title);
        const Term& findTerm(const std::string& title) const;
        Term& findTerm(const std::string& title);
        void selectTerm(const std::string& title);
};

#endif  // TERMCONTROLLER_HPP