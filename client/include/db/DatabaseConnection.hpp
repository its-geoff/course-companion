#ifndef DATABASECONNECTION_HPP
#define DATABASECONNECTION_HPP

/**
 * @file DatabaseConnection.hpp
 * @brief Definition of DatabaseConnection, which manages the MySQL session and initializes the schema.
 *
 * Wraps MySQL Connector/C++ X DevAPI session management. A single DatabaseConnection instance
 * is created at startup and passed by reference to each repository. Repositories do not own the session.
 *
 * Provides declarations only; see DatabaseConnection.cpp for implementations.
 */

#include <string>               // for connection parameters
#include <mysqlx/xdevapi.h>     // for MySQL Connector/C++ X DevAPI

/**
 * @class DatabaseConnection
 * @brief Owns the MySQL session and exposes it to repositories.
 *
 * Establishes a connection to a MySQL server on construction and runs CREATE TABLE IF NOT EXISTS
 * for all three tables (terms, courses, assignments) so the schema is always in sync without
 * any manual setup step.
 */
class DatabaseConnection {
    private:
        mysqlx::Session session_;
        mysqlx::Schema schema_;

        void initializeSchema();    // runs CREATE TABLE IF NOT EXISTS for all tables on construction

    public:
        DatabaseConnection(const std::string& host, unsigned int port, const std::string& user,
            const std::string& password, const std::string& schema);

        // non-copyable; one session, one owner
        DatabaseConnection(const DatabaseConnection&) = delete;
        DatabaseConnection& operator=(const DatabaseConnection&) = delete;

        mysqlx::Schema& getSchema();
        mysqlx::Session& getSession();
};

#endif  // DATABASECONNECTION_HPP