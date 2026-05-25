#include "controller/TermController.hpp"
#include "utils/utils.hpp"

const std::unordered_map<std::string, Term>& TermController::getTermList() const {
    return termList_;
}

std::string TermController::getTermId(const std::string& title) const {
    std::string titleLower = utils::stringLower(title);
    auto it = titleToId_.find(titleLower);

    if (it == titleToId_.end()) {
        throw std::out_of_range("Term not found.");
    }

    return it->second;
}

CourseController& TermController::getCourseController() {
    if (!courseController_.has_value()) {
        throw std::logic_error("No term selected.");
    }

    return *courseController_;
}

void TermController::addTerm(const std::string& title, const std::chrono::year_month_day& startDate,
    const std::chrono::year_month_day& endDate, bool active) {
    Term term{title, startDate, endDate, active};

    auto termIt = termList_.emplace(term.getId(), std::move(term)).first;
    auto titleInserted = titleToId_.emplace(utils::stringLower(termIt->second.getTitle()), termIt->first).second;

    if (!titleInserted) {
        termList_.erase(termIt);
        throw std::logic_error("Term with the same title already exists.");
    }
}

void TermController::editTitle(const std::string& id, const std::string& newTitle) {
    Term& term = termList_.at(id);
    std::string oldTitle = term.getTitle();

    auto inserted = titleToId_.emplace(utils::stringLower(newTitle), id).second;

    if (!inserted) {
        throw std::logic_error("A term with this title already exists.");
    }

    titleToId_.erase(utils::stringLower(oldTitle));
    term.setTitle(newTitle);
}

void TermController::editStartDate(const std::string& id, const std::chrono::year_month_day& newStartDate) {
    Term& term = termList_.at(id);
    term.setStartDate(newStartDate);
}

void TermController::editEndDate(const std::string& id, const std::chrono::year_month_day& newEndDate) {
    Term& term = termList_.at(id);
    term.setEndDate(newEndDate);
}

void TermController::editActive(const std::string& id, bool newActive) {
    Term& term = termList_.at(id);
    term.setActive(newActive);
}

void TermController::removeTerm(const std::string& title) {
    std::string id = getTermId(title);
    termList_.erase(id);
    titleToId_.erase(utils::stringLower(title));
}

const Term& TermController::findTerm(const std::string& title) const {
    std::string id = getTermId(title);
    return termList_.at(id);
}

Term& TermController::findTerm(const std::string& title) {
    std::string id = getTermId(title);
    return termList_.at(id);
}

void TermController::selectTerm(const std::string& title) {
    try {
        Term& termRef = findTerm(title);
        activeTerm_ = &termRef;
        courseController_.emplace(*activeTerm_);
    } catch (const std::out_of_range& e) {
        throw std::out_of_range("Term not found.");
    }
}