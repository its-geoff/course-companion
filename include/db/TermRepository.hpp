#ifndef TERMREPOSITORY_HPP
#define TERMREPOSITORY_HPP

/**
 * @file TermRepository.hpp
 * @brief Definition of TermRepository, which handles database operations for Term objects.
 *
 * Implements IDatabase<Term> with MySQL-backed CRUD operations. TermController depends on
 * IDatabase<Term> rather than this class directly, keeping the controller decoupled from SQL.
 *
 * Provides declarations only; see TermRepository.cpp for implementations.
 */

#include <string>               // for string variables
#include <vector>               // for findAll and findByParentId return types
#include <optional>             // for findById return type
#include <mysqlx/xdevapi.h>     // for MySQL Connector/C++ X DevAPI
#include "db/IDatabase.hpp"     // for IDatabase interface
#include "db/DatabaseConnection.hpp"    // for DatabaseConnection reference
#include "model/Term.hpp"       // for Term objects

/**
 * @class TermRepository
 * @brief MySQL-backed implementation of IDatabase<Term>.
 *
 * Translates between Term objects and rows in the terms table. Receives a DatabaseConnection
 * by reference and does not own the session.
 */
class TermRepository : public IDatabase<Term> {
    private:
        DatabaseConnection& db_;

        Term rowToTerm(const mysqlx::Row& row) const;   // converts a database row into a Term object

    public:
        TermRepository(DatabaseConnection& db);

        // non-copyable; holds a reference to a shared session
        TermRepository(const TermRepository&) = delete;
        TermRepository& operator=(const TermRepository&) = delete;

        void insert(const Term& term) override;
        void update(const Term& term) override;
        void remove(const std::string& id) override;
        std::optional<Term> findById(const std::string& id) override;   // returns empty if not found
        std::vector<Term> findAll() override;
        std::vector<Term> findByParentId(const std::string& parentId) override;     // not applicable for Term; throws logic_error
};

#endif  // TERMREPOSITORY_HPP