#include "db/DatabaseConnection.hpp"

/**
 * @file DatabaseConnection.cpp
 * @brief Implementation of DatabaseConnection, which manages the MySQL session and initializes the schema.
 *
 * Establishes a MySQL Connector/C++ X DevAPI session on construction and runs schema initialization
 * so repositories can assume all tables exist.
 *
 * Provides implementations only; see DatabaseConnection.hpp for definitions.
 */

// connects to MySQL, creates the database if needed, and initializes the schema
DatabaseConnection::DatabaseConnection(const std::string& host, unsigned int port, const std::string& user,
    const std::string& password, const std::string& schema)
    : session_(mysqlx::SessionOption::HOST, host,
               mysqlx::SessionOption::PORT, port,
               mysqlx::SessionOption::USER, user,
               mysqlx::SessionOption::PWD,  password),
      schema_(session_.getSchema(schema))
{
    // create the database if it does not exist, then re-bind schema
    session_.sql("CREATE DATABASE IF NOT EXISTS `" + schema + "`").execute();
    schema_ = session_.getSchema(schema);

    initializeSchema();
}

mysqlx::Schema& DatabaseConnection::getSchema() {
    return schema_;
}

mysqlx::Session& DatabaseConnection::getSession() {
    return session_;
}

// runs CREATE TABLE IF NOT EXISTS for terms, courses, and assignments
// computed fields (gradePct, letterGrade, gpaVal, totalCredits, ovrGpa) are excluded; derived by the model layer
void DatabaseConnection::initializeSchema() {
    session_.sql(R"(
        CREATE TABLE IF NOT EXISTS terms (
            id          CHAR(36)        NOT NULL PRIMARY KEY,
            title       VARCHAR(255)    NOT NULL UNIQUE,
            start_date  DATE            NOT NULL,
            end_date    DATE            NOT NULL,
            active      BOOLEAN         NOT NULL DEFAULT TRUE
        )
    )").execute();

    session_.sql(R"(
        CREATE TABLE IF NOT EXISTS courses (
            id            CHAR(36)        NOT NULL PRIMARY KEY,
            term_id       CHAR(36)        NOT NULL,
            title         VARCHAR(255)    NOT NULL,
            description   TEXT,
            start_date    DATE            NOT NULL,
            end_date      DATE            NOT NULL,
            num_credits   INT             NOT NULL DEFAULT 3,
            active        BOOLEAN         NOT NULL DEFAULT TRUE,
            UNIQUE (term_id, title),
            FOREIGN KEY (term_id) REFERENCES terms(id) ON DELETE CASCADE
        )
    )").execute();

    session_.sql(R"(
        CREATE TABLE IF NOT EXISTS assignments (
            id          CHAR(36)        NOT NULL PRIMARY KEY,
            course_id   CHAR(36)        NOT NULL,
            title       VARCHAR(255)    NOT NULL,
            description TEXT,
            category    VARCHAR(100)    NOT NULL,
            due_date    DATE            NOT NULL,
            completed   BOOLEAN         NOT NULL DEFAULT FALSE,
            grade       FLOAT           NOT NULL DEFAULT 0.0,
            UNIQUE (course_id, title),
            FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE
        )
    )").execute();
}