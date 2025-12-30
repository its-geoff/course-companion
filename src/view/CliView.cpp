#include "view/CliView.hpp"

/**
 * @file CliView.cpp
 * @brief Implementation of the CliView class, which handles I/O and user interaction for the command line interface (CLI).
 * 
 * This class collects user input and presents output to the user through the command line. It does not contain any 
 * app logic. The class sends information to the Controller and outputs results from the Controller. 
 * 
 * Provides implementations only; see CliView.hpp for definitions.
 */

#include "utils/utils.hpp"      // for reused custom functions

void CliView::displayMenu() const {
    
}

// prints a delimiter line to separate different sections of information
void CliView::displayDelim() const {
    out_ << "===========================================================" << "\n";
}

explicit CliView::CliView(Controller &controller, std::istream& is, std::ostream& os)
    : controller_{controller}, in_{is}, out_{os} {}