#ifndef TERMCONTROLLER_HPP
#define TERMCONTROLLER_HPP

/**
 * @file TermController.hpp
 * @brief Definition of a controller that manages interaction between Term and the views.
 * 
 * This controller defines functions that integrate the Term class with the views and allow for the 
 * model to connect to the main function.
 */

#include <string>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <optional>
#include <QObject>
#include "model/Term.hpp"
#include "controller/CourseController.hpp"

class TermController : public QObject {
    Q_OBJECT

    public:
        TermController() = default;
        TermController(const TermController&) = delete;
        TermController& operator=(const TermController&) = delete;

        const std::unordered_map<std::string, Term>& getTermList() const;
        const std::vector<std::string>& getTermOrder() const;
        std::string getTermId(const std::string& title) const;
        CourseController& getCourseController();
        const Term& getActiveTerm() const;

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

    signals:
        void dataChanged();
        void termSelected();

    private:
        std::unordered_map<std::string, Term> termList_{};
        std::unordered_map<std::string, std::string> titleToId_{};
        std::vector<std::string> termOrder_{};
        Term* activeTerm_ = nullptr;
        std::optional<CourseController> courseController_{};
};

#endif  // TERMCONTROLLER_HPP