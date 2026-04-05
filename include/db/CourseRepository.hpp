#ifndef COURSEREPOSITORY_HPP
#define COURSEREPOSITORY_HPP

/**
 * @file CourseRepository.hpp
 * @brief Definition of CourseRepository, which handles database operations for Course objects.
 *
 * Implements IDatabase<Course> with MySQL-backed CRUD operations. CourseController depends on
 * IDatabase<Course> rather than this class directly, keeping the controller decoupled from SQL.
 *
 * Provides declarations only; see CourseRepository.cpp for implementations.
 */

#include <string>               // for string variables
#include <vector>               // for findAll and findByParentId return types
#include <optional>             // for findById return type
#include <mysqlx/xdevapi.h>     // for MySQL Connector/C++ X DevAPI
#include "db/IDatabase.hpp"     // for IDatabase interface
#include "db/DatabaseConnection.hpp"    // for DatabaseConnection reference
#include "model/Course.hpp"     // for Course objects

/**
 * @class CourseRepository
 * @brief MySQL-backed implementation of IDatabase<Course>.
 *
 * Translates between Course objects and rows in the courses table. Receives a DatabaseConnection
 * by reference and does not own the session.
 */
class CourseRepository : public IDatabase<Course> {
    private:
        DatabaseConnection& db_;
        std::string termId_;    // id of the parent term; used in insert to set term_id foreign key

        Course rowToCourse(const mysqlx::Row& row) const;  // converts a database row into a Course object

    public:
        CourseRepository(DatabaseConnection& db, const std::string& termId);

        // non-copyable; holds a reference to a shared session
        CourseRepository(const CourseRepository&) = delete;
        CourseRepository& operator=(const CourseRepository&) = delete;

        void insert(const Course& course) override;
        void update(const Course& course) override;
        void remove(const std::string& id) override;
        std::optional<Course> findById(const std::string& id) override;     // returns empty if not found
        std::vector<Course> findAll() override;
        std::vector<Course> findByParentId(const std::string& termId) override;     // returns all courses belonging to a term
};

#endif  // COURSEREPOSITORY_HPP