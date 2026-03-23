#include "db/TermRepository.hpp"

/**
 * @file TermRepository.cpp
 * @brief Implementation of TermRepository, which handles database operations for Term objects.
 *
 * Translates between Term objects and rows in the terms table using MySQL Connector/C++.
 *
 * Provides implementations only; see TermRepository.hpp for definitions.
 */

#include <stdexcept>    // for exceptions
#include <sstream>      // for date string parsing

TermRepository::TermRepository(DatabaseConnection& db) : db_{db} {}

// converts a database row into a Term object
Term TermRepository::rowToTerm(const mysqlx::Row& row) const {
    std::string id          = row[0].get<std::string>();
    std::string title       = row[1].get<std::string>();
    std::string startStr    = row[2].get<std::string>();
    std::string endStr      = row[3].get<std::string>();
    bool active             = row[4].get<bool>();

    // parse date strings from YYYY-MM-DD format into year_month_day
    int sy, sm, sd, ey, em, ed;
    sscanf(startStr.c_str(), "%d-%d-%d", &sy, &sm, &sd);
    sscanf(endStr.c_str(), "%d-%d-%d", &ey, &em, &ed);

    std::chrono::year_month_day startDate = std::chrono::year{sy}/std::chrono::month{static_cast<unsigned>(sm)}/std::chrono::day{static_cast<unsigned>(sd)};
    std::chrono::year_month_day endDate = std::chrono::year{ey}/std::chrono::month{static_cast<unsigned>(em)}/std::chrono::day{static_cast<unsigned>(ed)};

    return Term{title, startDate, endDate, active};
}

// inserts a new Term row into the terms table
void TermRepository::insert(const Term& term) {
    std::ostringstream start, end;
    start << term.getStartDate();
    end << term.getEndDate();

    db_.getSession().sql(
        "INSERT INTO terms (id, title, start_date, end_date, active) VALUES (?, ?, ?, ?, ?)"
    ).bind(term.getId(), term.getTitle(), start.str(), end.str(), term.getActive()).execute();
}

// updates an existing Term row matched by id
void TermRepository::update(const Term& term) {
    std::ostringstream start, end;
    start << term.getStartDate();
    end << term.getEndDate();

    db_.getSession().sql(
        "UPDATE terms SET title = ?, start_date = ?, end_date = ?, active = ? WHERE id = ?"
    ).bind(term.getTitle(), start.str(), end.str(), term.getActive(), term.getId()).execute();
}

// deletes a Term row matched by id
void TermRepository::remove(const std::string& id) {
    db_.getSession().sql(
        "DELETE FROM terms WHERE id = ?"
    ).bind(id).execute();
}

// returns a Term matched by id, or empty if not found
std::optional<Term> TermRepository::findById(const std::string& id) {
    auto result = db_.getSession().sql(
        "SELECT id, title, start_date, end_date, active FROM terms WHERE id = ?"
    ).bind(id).execute();

    auto row = result.fetchOne();

    if (row.isNull()) {
        return std::nullopt;
    }

    return rowToTerm(row);
}

// returns all Term rows from the terms table
std::vector<Term> TermRepository::findAll() {
    auto result = db_.getSession().sql(
        "SELECT id, title, start_date, end_date, active FROM terms"
    ).execute();

    std::vector<Term> terms;

    for (auto row : result.fetchAll()) {
        terms.push_back(rowToTerm(row));
    }

    return terms;
}

// not applicable for Term — terms have no parent
std::vector<Term> TermRepository::findByParentId(const std::string& parentId) {
    throw std::logic_error("TermRepository does not support findByParentId.");
}