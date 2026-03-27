#include "controller/TermController.hpp"
#include "utils/utils.hpp"

TermController::TermController(DatabaseConnection& db)
    : db_{&db}, termRepo_{std::make_unique<TermRepository>(db)} {}

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

void TermController::loadFromDb() {
    std::vector<Term> terms = termRepo_->findAll();

    for (Term& term : terms) {
        auto [termIt, termInserted] = termList_.emplace(term.getId(), std::move(term));
        titleToId_.emplace(utils::stringLower(termIt->second.getTitle()), termIt->first);
    }
}

void TermController::addTerm(const std::string& title, const std::chrono::year_month_day& startDate,
    const std::chrono::year_month_day& endDate, bool active) {
    Term term{title, startDate, endDate, active};

    auto [termIt, termInserted] = termList_.emplace(term.getId(), std::move(term));
    auto [titleIt, titleInserted] = titleToId_.emplace(utils::stringLower(termIt->second.getTitle()), termIt->first);

    if (!titleInserted) {
        termList_.erase(termIt);
        throw std::logic_error("Term with the same title already exists.");
    }

    if (termRepo_) {
        termRepo_->insert(termIt->second);
    }
}

void TermController::editTitle(const std::string& id, const std::string& newTitle) {
    Term& term = termList_.at(id);
    std::string oldTitle = term.getTitle();

    auto [_, inserted] = titleToId_.emplace(utils::stringLower(newTitle), id);

    if (!inserted) {
        throw std::logic_error("A term with this title already exists.");
    }

    titleToId_.erase(utils::stringLower(oldTitle));
    term.setTitle(newTitle);

    if (termRepo_) {
        termRepo_->update(term);
    }
}

void TermController::editStartDate(const std::string& id, const std::chrono::year_month_day& newStartDate) {
    Term& term = termList_.at(id);
    term.setStartDate(newStartDate);

    if (termRepo_) {
        termRepo_->update(term);
    }
}

void TermController::editEndDate(const std::string& id, const std::chrono::year_month_day& newEndDate) {
    Term& term = termList_.at(id);
    term.setEndDate(newEndDate);

    if (termRepo_) {
        termRepo_->update(term);
    }
}

void TermController::editActive(const std::string& id, bool newActive) {
    Term& term = termList_.at(id);
    term.setActive(newActive);

    if (termRepo_) {
        termRepo_->update(term);
    }
}

void TermController::removeTerm(const std::string& title) {
    std::string id = getTermId(title);

    if (termRepo_) {
        termRepo_->remove(id);
    }

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
        if (db_) {
            courseController_.emplace(*activeTerm_, *db_);
        } else {
            courseController_.emplace(*activeTerm_);
        }
    } catch (const std::exception& e) {
        throw std::out_of_range("Term not found.");
    }
}