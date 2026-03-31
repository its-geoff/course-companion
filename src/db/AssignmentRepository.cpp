#include "db/AssignmentRepository.hpp"

/**
 * @file AssignmentRepository.cpp
 * @brief Implementation of AssignmentRepository, which handles database operations for Assignment objects.
 *
 * Translates between Assignment objects and rows in the assignments table using MySQL Connector/C++ X DevAPI.
 *
 * Provides implementations only; see AssignmentRepository.hpp for definitions.
 */

#include <stdexcept>    // for exceptions
#include <sstream>      // for date string formatting
#include "utils/utils.hpp"  // for reused custom functions

AssignmentRepository::AssignmentRepository(DatabaseConnection& db, const std::string& courseId) : db_{db}, courseId_{courseId} {}

// converts a database row into an Assignment object
// column order: id(0), course_id(1), title(2), description(3), category(4), due_date(5), completed(6), grade(7)
Assignment AssignmentRepository::rowToAssignment(const mysqlx::Row& row) const {
    std::string dueDateStr = row[5].get<std::string>();

    // parse date string from YYYY-MM-DD format into year_month_day
    std::chrono::year_month_day dueDate = utils::parseDate(dueDateStr);

    // description is optional; use empty string if null
    std::string description = row[3].isNull() ? "" : row[3].get<std::string>();

    return Assignment::fromRow(row[0].get<std::string>(), row[2].get<std::string>(), description, 
        row[4].get<std::string>(), dueDate, row[6].get<bool>(), row[7].get<float>());
}

// inserts a new Assignment row into the assignments table
void AssignmentRepository::insert(const Assignment& assignment) {
    std::ostringstream due;
    due << assignment.getDueDate();

    db_.getSession().sql(
        "INSERT INTO assignments (id, course_id, title, description, category, due_date, completed, grade) VALUES (?, ?, ?, ?, ?, ?, ?, ?)"
    ).bind(assignment.getId(), courseId_, assignment.getTitle(), assignment.getDescription(), assignment.getCategory(), due.str(), assignment.getCompleted(), assignment.getGrade()).execute();
}

// updates an existing Assignment row matched by id
void AssignmentRepository::update(const Assignment& assignment) {
    std::ostringstream due;
    due << assignment.getDueDate();

    db_.getSession().sql(
        "UPDATE assignments SET title = ?, description = ?, category = ?, due_date = ?, completed = ?, grade = ? WHERE id = ?"
    ).bind(assignment.getTitle(), assignment.getDescription(), assignment.getCategory(), due.str(), assignment.getCompleted(), assignment.getGrade(), assignment.getId()).execute();
}

// deletes an Assignment row matched by id
void AssignmentRepository::remove(const std::string& id) {
    db_.getSession().sql(
        "DELETE FROM assignments WHERE id = ?"
    ).bind(id).execute();
}

// returns an Assignment matched by id, or empty if not found
std::optional<Assignment> AssignmentRepository::findById(const std::string& id) {
    auto result = db_.getSession().sql(
        "SELECT id, course_id, title, description, category, DATE_FORMAT(due_date, '%Y-%m-%d'), completed, grade FROM assignments WHERE id = ?"
    ).bind(id).execute();

    auto row = result.fetchOne();

    if (row.isNull()) {
        return std::nullopt;
    }

    return rowToAssignment(row);
}

// returns all Assignment rows from the assignments table
std::vector<Assignment> AssignmentRepository::findAll() {
    auto result = db_.getSession().sql(
        "SELECT id, course_id, title, description, category, DATE_FORMAT(due_date, '%Y-%m-%d'), completed, grade FROM assignments "
    ).execute();

    std::vector<Assignment> assignments;

    for (auto row : result.fetchAll()) {
        assignments.push_back(rowToAssignment(row));
    }

    return assignments;
}

// returns all Assignment rows belonging to the given course id
std::vector<Assignment> AssignmentRepository::findByParentId(const std::string& courseId) {
    auto result = db_.getSession().sql(
        "SELECT id, course_id, title, description, category, DATE_FORMAT(due_date, '%Y-%m-%d'), \
        completed, grade FROM assignments WHERE course_id = ?"
    ).bind(courseId).execute();

    std::vector<Assignment> assignments;

    for (auto row : result.fetchAll()) {
        assignments.push_back(rowToAssignment(row));
    }

    return assignments;
}