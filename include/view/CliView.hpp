#ifndef CLIVIEW_HPP
#define CLIVIEW_HPP

/**
 * @file CliView.hpp
 * @brief Definition of the CliView class, which handles I/O and user interaction for the command line interface (CLI).
 * 
 * This class collects user input and presents output to the user through the command line. It does not contain any 
 * app logic. The class sends information to the Controller and outputs results from the Controller.
 * 
 * Provides declarations only; see CliView.cpp for implementations.
 */

#include <iostream>         // for i/o streams
#include <chrono>           // for date and time-related variables
#include <vector>           // for vector of changed fields
#include <optional>         // for selected Term, Course, and Assignment
#include <functional>       // for reference wrapper
#include <unordered_map>    // for Term objects
#include "controller/TermController.hpp"

/**
 * @class CliView
 * @brief Represents a view for the command line interface CLI).
 * 
 * Takes input from the user and prints output to the command line. Does not contain app logic. Interacts with the 
 * Controller to provide input to model functions and displays output after receiving return values.
 */
class CliView {
    private:
        TermController &controller_;
        // may or may not exist
        std::optional<std::reference_wrapper<Term>> selectedTerm_;
        std::optional<std::reference_wrapper<Course>> selectedCourse_;
        std::optional<std::reference_wrapper<Assignment>> selectedAssignment_;
        std::istream &in_;
        std::ostream &out_;
        enum class MenuState {
            term,
            course,
            assignment,
            exit
        };

        std::chrono::year_month_day parseDate(const std::string &input) const;
        std::vector<std::string> splitStringByComma(const std::string &str);

        void displayDelim() const;
        void displaySecondaryDelim() const;
        void displayIntro() const;
        void displayTermMenu() const;
        void displayCourseMenu() const;
        void displayAssignmentMenu() const;
        void displayTermListInfo() const;
        void displayCourseListInfo() const;
        // void displayAssignmentListInfo() const;
        void displayInvalidSelection() const;

        void promptAddTerm();
        void promptEditTerm();
        void promptSelectTerm();
        void promptRemoveTerm();
        void promptAddCourse();
        void promptEditCourse();
        void promptSelectCourse();
        void promptRemoveCourse();

        char getCharInput(const std::string &label, const char defaultVal) const;
        std::string getStringInput(const std::string &label, const std::string &defaultVal) const;
        std::chrono::year_month_day getDateInput(const std::string &label, const std::chrono::year_month_day defaultVal) const;
        float getFloatInput(const std::string &label, const float defaultVal) const;
        int getIntInput(const std::string &label, const int defaultVal) const;
        bool getBoolInput(const std::string &label, const bool defaultVal) const;

    public:
        explicit CliView(TermController &controller, std::istream& is = std::cin, std::ostream& os = std::cout);
        void run();
};

#endif  // CLIVIEW_HPP