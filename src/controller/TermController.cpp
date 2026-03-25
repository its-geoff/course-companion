#include "controller/TermController.hpp"

/**
 * @file TermController.cpp
 * @brief Implementation of a controller that manages interaction between the views and Term. 
 * 
 * This controller implements functions that integrate the classes together and allow for code 
 * reusability within the main function.
 */

#include "utils/utils.hpp"      // for stringLower

TermController::TermController(DatabaseConnection& db) : db_{db}, termRepo_{db} {}

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

// loads all terms from the database into termList_ on startup
void TermController::loadFromDb() {
    std::vector<Term> terms = termRepo_.findAll();

    for (Term& term : terms) {
        auto [termIt, termInserted] = termList_.emplace(term.getId(), std::move(term));
        titleToId_.emplace(utils::stringLower(termIt->second.getTitle()), termIt->first);
    }
}

// uses info from view to create a Term object, adds it to the list of terms, and persists it to the database
void TermController::addTerm(const std::string& title, const std::chrono::year_month_day& startDate, 
    const std::chrono::year_month_day& endDate, bool active) {
    Term term{title, startDate, endDate, active};

    auto [termIt, termInserted] = termList_.emplace(term.getId(), std::move(term));
    auto [titleIt, titleInserted] = titleToId_.emplace(utils::stringLower(termIt->second.getTitle()), termIt->first);

    if (!titleInserted) {
        termList_.erase(termIt);
        throw std::logic_error("Term with the same title already exists.");
    }

    termRepo_.insert(termIt->second);
}

// edits the title of the Term with the given ID and syncs to the database
void TermController::editTitle(const std::string& id, const std::string& newTitle) {
    Term& term = termList_.at(id);
    std::string oldTitle = term.getTitle();

    auto [_, inserted] = titleToId_.emplace(utils::stringLower(newTitle), id);

    if (!inserted) {
        throw std::logic_error("A term with this title already exists.");
    }

    titleToId_.erase(utils::stringLower(oldTitle));
    term.setTitle(newTitle);
    termRepo_.update(term);
}

// edits the start date of the Term with the given ID and syncs to the database
void TermController::editStartDate(const std::string& id, const std::chrono::year_month_day& newStartDate) {
    Term& term = termList_.at(id);
    term.setStartDate(newStartDate);
    termRepo_.update(term);
}

// edits the end date of the Term with the given ID and syncs to the database
void TermController::editEndDate(const std::string& id, const std::chrono::year_month_day& newEndDate) {
    Term& term = termList_.at(id);
    term.setEndDate(newEndDate);
    termRepo_.update(term);
}

// edits the active flag of the Term with the given ID and syncs to the database
void TermController::editActive(const std::string& id, bool newActive) {
    Term& term = termList_.at(id);
    term.setActive(newActive);
    termRepo_.update(term);
}

// searches title -> id, erases the named Term from the list, and removes it from the database
void TermController::removeTerm(const std::string& title) {
    std::string id = getTermId(title);
    termRepo_.remove(id);
    termList_.erase(id);
    titleToId_.erase(utils::stringLower(title));
}

// finds a Term in termList based on title; non-mutable (read-only)
const Term& TermController::findTerm(const std::string& title) const {
    std::string id = getTermId(title);
    return termList_.at(id);
}

// finds a Term in termList based on title; mutable (read and write access)
Term& TermController::findTerm(const std::string& title) {
    std::string id = getTermId(title);
    return termList_.at(id);
}

// selects a term and makes it "active", creating a CourseController for that term
void TermController::selectTerm(const std::string& title) {
    try {
        Term& termRef = findTerm(title);
        activeTerm_ = &termRef;
        courseController_.emplace(*activeTerm_, db_);
    } catch (const std::exception& e) {
        throw std::out_of_range("Term not found.");
    }
}