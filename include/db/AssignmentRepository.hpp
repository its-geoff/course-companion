#ifndef ASSIGNMENTREPOSITORY_HPP
#define ASSIGNMENTREPOSITORY_HPP

/**
 * @file AssignmentRepository.hpp
 * @brief Definition of AssignmentRepository, which handles database operations for Assignment objects.
 *
 * Implements IDatabase<Assignment> with MySQL-backed CRUD operations. AssignmentController depends on
 * IDatabase<Assignment> rather than this class directly, keeping the controller decoupled from SQL.
 *
 * Provides declarations only; see AssignmentRepository.cpp for implementations.
 */

#include <string>               // for string variables
#include <vector>               // for findAll and findByParentId return types
#include <optional>             // for findById return type
#include <mysqlx/xdevapi.h>     // for MySQL Connector/C++ X DevAPI
#include "db/IDatabase.hpp"     // for IDatabase interface
#include "db/DatabaseConnection.hpp"    // for DatabaseConnection reference
#include "model/Assignment.hpp" // for Assignment objects

/**
 * @class AssignmentRepository
 * @brief MySQL-backed implementation of IDatabase<Assignment>.
 *
 * Translates between Assignment objects and rows in the assignments table. Receives a DatabaseConnection
 * by reference and does not own the session.
 */
class AssignmentRepository : public IDatabase<Assignment> {
    private:
        DatabaseConnection& db_;
        std::string courseId_;  // id of the parent course; used in insert to set course_id foreign key

        Assignment rowToAssignment(const mysqlx::Row& row) const;  // converts a database row into an Assignment object

    public:
        AssignmentRepository(DatabaseConnection& db, const std::string& courseId);

        // non-copyable; holds a reference to a shared session
        AssignmentRepository(const AssignmentRepository&) = delete;
        AssignmentRepository& operator=(const AssignmentRepository&) = delete;

        void insert(const Assignment& assignment) override;
        void update(const Assignment& assignment) override;
        void remove(const std::string& id) override;
        std::optional<Assignment> findById(const std::string& id) override;     // returns empty if not found
        std::vector<Assignment> findAll() override;
        std::vector<Assignment> findByParentId(const std::string& courseId) override;   // returns all assignments belonging to a course
};

#endif  // ASSIGNMENTREPOSITORY_HPP