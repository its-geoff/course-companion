#include "controller/TermController.hpp"

/**
 * @file TermController.cpp
 * @brief Implementation of a controller that manages interaction between the views and Term. 
 * 
 * This controller implements functions that integrate the classes together and allow for code 
 * reusability within the main function.
 */

#include "utils/utils.hpp"      // for stringLower

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

// uses info from CliView to create a Term object then add it to the list of terms
void TermController::addTerm(const std::string& title, const std::chrono::year_month_day& startDate, 
    const std::chrono::year_month_day& endDate, bool active) {
    Term term{title, startDate, endDate, active};

    auto [termIt, termInserted] = termList_.emplace(term.getId(), std::move(term));
    // make title lowercase in titleToId for easier comparison
    auto [titleIt, titleInserted] = titleToId_.emplace(utils::stringLower(termIt->second.getTitle()), termIt->first);

    if (!titleInserted) {
        termList_.erase(termIt);   // erase Term object if there's an error
        throw std::logic_error("Term with the same title already exists.");
    }
}

// edits the title of the Term with the given ID
void TermController::editTitle(const std::string& id, const std::string& newTitle) {
    Term& term = termList_.at(id);
    std::string oldTitle = term.getTitle();

    auto [_, inserted] = titleToId_.emplace(utils::stringLower(newTitle), id);

    if (!inserted) {
        throw std::logic_error("A term with this title already exists.");
    }

    // change the title -> id mapping for the new title
    titleToId_.erase(utils::stringLower(oldTitle));

    // set title after insertion to title -> id mapping
    term.setTitle(newTitle);
}

// edits the start date of the Term with the given ID
void TermController::editStartDate(const std::string& id, const std::chrono::year_month_day& newStartDate) {
    Term& term = termList_.at(id);
    term.setStartDate(newStartDate);
}

// edits the end date of the Term with the given ID
void TermController::editEndDate(const std::string& id, const std::chrono::year_month_day& newEndDate) {
    Term& term = termList_.at(id);
    term.setEndDate(newEndDate);
}

// edits the active bool of the term with the given ID
void TermController::editActive(const std::string& id, bool newActive) {
    Term& term = termList_.at(id);
    term.setActive(newActive);
}

// searches title -> id and erases the named Term from the list
void TermController::removeTerm(const std::string& title) {
    std::string id = getTermId(title);
    termList_.erase(id);
    titleToId_.erase(utils::stringLower(title));
}

// find a Term in termList based on title; non-mutable (read-only)
const Term& TermController::findTerm(const std::string& title) const {
    std::string id = getTermId(title);
    return termList_.at(id);
}

// find a Term in termList based on title; mutable (read and write access)
Term& TermController::findTerm(const std::string& title) {
    std::string id = getTermId(title);
    return termList_.at(id);
}

// selects a term and makes it "active", creating a CourseController for that term
void TermController::selectTerm(const std::string& title) {
    try {
        Term& termRef = findTerm(title);
        activeTerm_ = &termRef;
        courseController_.emplace(*activeTerm_);
    } catch (const std::exception& e) {
        throw std::out_of_range("Term not found.");
    }
}