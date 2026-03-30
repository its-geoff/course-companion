#include "db/CourseRepository.hpp"

/**
 * @file CourseRepository.cpp
 * @brief Implementation of CourseRepository, which handles database operations for Course objects.
 *
 * Translates between Course objects and rows in the courses table using MySQL Connector/C++ X DevAPI.
 *
 * Provides implementations only; see CourseRepository.hpp for definitions.
 */

#include <stdexcept>    // for exceptions
#include <sstream>      // for date string formatting
#include "utils/utils.hpp"  // for reused custom functions

CourseRepository::CourseRepository(DatabaseConnection& db, const std::string& termId) : db_{db}, termId_{termId} {}

// converts a database row into a Course object
// column order: id(0), term_id(1), title(2), description(3), start_date(4), end_date(5), num_credits(6), active(7)
Course CourseRepository::rowToCourse(const mysqlx::Row& row) const {
    std::string startStr    = row[4].get<std::string>();
    std::string endStr      = row[5].get<std::string>();

    // parse date strings from YYYY-MM-DD format into year_month_day
    std::chrono::year_month_day startDate = utils::parseDate(startStr);
    std::chrono::year_month_day endDate = utils::parseDate(endStr);

    // description is optional; use empty string if null
    std::string description = row[3].isNull() ? "" : row[3].get<std::string>();

    return Course::fromRow(row[0].get<std::string>(), row[2].get<std::string>(), description, startDate, 
        endDate, row[6].get<int>(), row[7].get<bool>());
}

// inserts a new Course row into the courses table
void CourseRepository::insert(const Course& course) {
    std::ostringstream start, end;
    start << course.getStartDate();
    end << course.getEndDate();

    db_.getSession().sql(
        "INSERT INTO courses (id, term_id, title, description, start_date, end_date, num_credits, active) VALUES (?, ?, ?, ?, ?, ?, ?, ?)"
    ).bind(course.getId(), termId_, course.getTitle(), course.getDescription(), start.str(), end.str(), course.getNumCredits(), course.getActive()).execute();
}

// updates an existing Course row matched by id
void CourseRepository::update(const Course& course) {
    std::ostringstream start, end;
    start << course.getStartDate();
    end << course.getEndDate();

    db_.getSession().sql(
        "UPDATE courses SET title = ?, description = ?, start_date = ?, end_date = ?, num_credits = ?, active = ? WHERE id = ?"
    ).bind(course.getTitle(), course.getDescription(), start.str(), end.str(), course.getNumCredits(), course.getActive(), course.getId()).execute();
}

// deletes a Course row matched by id
void CourseRepository::remove(const std::string& id) {
    db_.getSession().sql(
        "DELETE FROM courses WHERE id = ?"
    ).bind(id).execute();
}

// returns a Course matched by id, or empty if not found
std::optional<Course> CourseRepository::findById(const std::string& id) {
    auto result = db_.getSession().sql(
        "SELECT id, term_id, title, description, start_date, end_date, num_credits, active FROM courses WHERE id = ?"
    ).bind(id).execute();

    auto row = result.fetchOne();

    if (row.isNull()) {
        return std::nullopt;
    }

    return rowToCourse(row);
}

// returns all Course rows from the courses table
std::vector<Course> CourseRepository::findAll() {
    auto result = db_.getSession().sql(
        "SELECT id, term_id, title, description, start_date, end_date, num_credits, active FROM courses"
    ).execute();

    std::vector<Course> courses;

    for (auto row : result.fetchAll()) {
        courses.push_back(rowToCourse(row));
    }

    return courses;
}

// returns all Course rows belonging to the given term id
std::vector<Course> CourseRepository::findByParentId(const std::string& termId) {
    auto result = db_.getSession().sql(
        "SELECT id, term_id, title, description, start_date, end_date, num_credits, active FROM courses WHERE term_id = ?"
    ).bind(termId).execute();

    std::vector<Course> courses;

    for (auto row : result.fetchAll()) {
        courses.push_back(rowToCourse(row));
    }

    return courses;
}