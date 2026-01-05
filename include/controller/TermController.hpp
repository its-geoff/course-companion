#ifndef TERMCONTROLLER_HPP
#define TERMCONTROLLER_HPP

/**
 * @file TermController.hpp
 * @brief Definition of a controller that manages interaction between Term and the views.
 * 
 * This controller defines functions that integrate the Term class with the views and allow for the 
 * model to connect to the main function.
 */

#include <string>           // for string variables
#include <chrono>           // for date and time-related variables
#include <unordered_map>    // for Term objects
#include "model/Term.hpp"

/**
 * @class TermController
 * @brief Controls the interaction between the Term class and the views.
 * 
 * Manages the program’s workflow by coordinating interactions between the user interface and the Term class. 
 * It stores the current application state and processes user input. This class completes delegated operations 
 * from the views and provides output to the user through the main function.
 */
class TermController {
    private:
        std::unordered_map<std::string, Term> termList_{};     // id -> Term
        std::unordered_map<std::string, std::string> titleToId_{};   // title -> id, titles in lowercase for easier comparison

    public:
        const Term& getTerm(const std::string& title) const;
        const std::unordered_map<std::string, Term>& getTermList() const;
        std::string getTermId(const std::string& title) const;

        void addTerm(const std::string& title, const std::chrono::year_month_day& startDate, const std::chrono::year_month_day& endDate, bool active);
        void editTitle(const std::string& id, const std::string& newTitle);
        void editStartDate(const std::string& id, const std::chrono::year_month_day& newStartDate);
        void editEndDate(const std::string& id, const std::chrono::year_month_day& newEndDate);
        void editActive(const std::string& id, bool active);
        void removeTerm(const std::string& title);
};

#endif  // TERMCONTROLLER_HPP