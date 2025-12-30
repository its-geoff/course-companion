#ifndef CLIVIEW_H
#define CLIVIEW_H

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
#include "controller/Controller.hpp"

/**
 * @class CliView
 * @brief Represents a view for the command line interface CLI).
 * 
 * Takes input from the user and prints output to the command line. Does not contain app logic. Interacts with the 
 * Controller to provide input to model functions and displays output after receiving return values.
 */
class CliView {
    private:
        Controller &controller_;
        std::istream &in_;
        std::ostream &out_;

        void displayMenu() const;
        void displayDelim() const;

        std::string getStringInput(const std::string &input) const;
        float getFloatInput(const std::string &input) const;
        int getIntInput(const std::string &input) const;
        bool getBoolInput(const std::string &input);

    public:
        explicit CliView(Controller &controller, std::istream& is = std::cin, std::ostream& os = std::cout);
        void run();
};

#endif