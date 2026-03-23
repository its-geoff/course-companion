#ifndef IDATABASE_HPP
#define IDATABASE_HPP

/**
 * @file IDatabase.hpp
 * @brief Definition of IDatabase, a template interface that standardizes CRUD operations for all models.
 *
 * Each model (Term, Course, Assignment) has a concrete repository that implements this interface.
 * Controllers depend on IDatabase<T> rather than concrete repository types, keeping them decoupled
 * from the database layer.
 *
 * Because this is a template, the full definition must live in the header — no .cpp file is needed.
 */

#include <string>       // for string variables
#include <vector>       // for findAll and findByParentId return types
#include <optional>     // for findById return type

/**
 * @class IDatabase
 * @brief Template interface for CRUD operations on a single model type.
 *
 * Concrete repositories (TermRepository, CourseRepository, AssignmentRepository) inherit from this
 * interface and provide MySQL-backed implementations. A mock implementation can be substituted for
 * testing without touching the database. T is the model type (Term, Course, or Assignment).
 */
template <typename T>
class IDatabase {
    public:
        virtual ~IDatabase() = default;

        virtual void insert(const T& entity) = 0;
        virtual void update(const T& entity) = 0;
        virtual void remove(const std::string& id) = 0;
        virtual std::optional<T> findById(const std::string& id) = 0;          // returns empty if not found
        virtual std::vector<T> findAll() = 0;
        virtual std::vector<T> findByParentId(const std::string& parentId) = 0;
};

#endif  // IDATABASE_HPP