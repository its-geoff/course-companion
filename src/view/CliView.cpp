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

#include <cstdio>       // for sscanf
#include <algorithm>    // for transform
#include <cctype>       // for tolower
#include <sstream>      // for stringstream variables
#include <exception>    // for exception handling
#include "controller/TermController.hpp"    // connection to TermController
#include "utils/utils.hpp"      // for reused custom functions

// takes date as string input and converts to year_month_day format
std::chrono::year_month_day CliView::parseDate(const std::string &input) const {
    int y, m, d;

    if (sscanf(input.c_str(), "%d-%d-%d", &y, &m, &d) == 3) {
        return std::chrono::year{y}/std::chrono::month{static_cast<unsigned>(m)}/std::chrono::day{static_cast<unsigned>(d)};
    }

    throw std::invalid_argument("Invalid date format. Expected YYYY-MM-DD.");
}

// splits a string in list format into a vector with multiple elements
std::vector<std::string> CliView::splitStringByComma(const std::string& str) {
    std::vector<std::string> words;
    std::stringstream ss{str};
    std::string word;

    // extract data until comma is found, then push into vector
    while (std::getline(ss, word, ',')) {
        words.push_back(word);
    }

    return words;
}

// prints a delimiter line to separate different sections of information
void CliView::displayDelim() const {
    out_ << "===================================================================================================" << "\n";
}

// prints a delimiter line visually different from the first to separate subsections of information
void CliView::displaySecondaryDelim() const {
    out_ << "---------------------------------------------------------------------------------------------------" << "\n";
}

// prints an introduction to the application
void CliView::displayIntro() const {
    displayDelim();
    out_ << "                                   Welcome to Course Companion!                                    " << "\n";
    out_ << "                               It's your personal academic sidekick.                               " << "\n";
}

// prints a menu of choices for the user to select a Term
void CliView::displayTermMenu() const {
    displayDelim();
    out_ << "                                             Term Menu                                             " << "\n";
    out_ << "                                 What would you like to do today?                                  " << "\n";
    out_ << "[A] Add term" << "\n";
    out_ << "[E] Edit term" << "\n";
    out_ << "[S] Select term" << "\n";
    out_ << "[R] Remove term" << "\n";
    out_ << "[X] Exit program" << "\n";
}

// prints a menu of choices for the user to select a Course
void CliView::displayCourseMenu() const {
    displayDelim();
    out_ << "Term: " << selectedTerm_->get().getTitle() << "\n";
    displayDelim();
    out_ << "                                            Course Menu                                            " << "\n";
    out_ << "                                 What would you like to do today?                                  " << "\n";
    out_ << "[A] Add course" << "\n";
    out_ << "[E] Edit course" << "\n";
    out_ << "[S] Select course" << "\n";
    out_ << "[R] Remove course" << "\n";
    out_ << "[X] Exit to term menu" << "\n";
}

// prints a menu of choices for the user to select an Assignment
void CliView::displayAssignmentMenu() const {
    displayDelim();
    out_ << "Term: " << selectedTerm_->get().getTitle() << "\n";
    out_ << "Course: " << "\n";
    displayDelim();
    out_ << "                                          Assignment Menu                                          " << "\n";
    out_ << "                                 What would you like to do today?                                  " << "\n";
    out_ << "[A] Add assignment" << "\n";
    out_ << "[E] Edit assignment" << "\n";
    out_ << "[S] Select assignment" << "\n";
    out_ << "[R] Remove assignment" << "\n";
    out_ << "[X] Exit to course menu" << "\n";
}

// displays information about all terms from TermController
void CliView::displayTermListInfo() const {
    const std::unordered_map<std::string, Term>& terms = controller_.getTermList();

    for (const auto& [id, term] : terms) {
        displaySecondaryDelim();
        term.printTermInfo(out_);
        displaySecondaryDelim();
    }
}

// prompt the user for information to add a term to the list
void CliView::promptAddTerm() {
    out_ << "Enter the following information for the term you'd like to add: " << "\n";
    std::string title = getStringInput("Title", " ");
    std::chrono::year_month_day startDate = getDateInput("Start Date (YYYY-MM-DD)", {});
    std::chrono::year_month_day endDate = getDateInput("End Date (YYYY-MM-DD)", {});
    bool active = getBoolInput("Current term? (yes/no)", true);

    controller_.addTerm(title, startDate, endDate, active);
    out_ << "Term '" << title << "' successfully added." << "\n";
}

// prompt the user for the information to edit a term in the current list
void CliView::promptEditTerm() {
    bool editTitle{false};
    bool editStartDate{false};
    bool editEndDate{false};
    bool editActive{false};
    std::vector<std::string> editFields{};

    out_ << "Enter the following information for the term you'd like to edit: " << "\n";
    std::string title = getStringInput("Title", " ");
    selectedTerm_ = controller_.getTerm(title);
    std::string id = selectedTerm_->get().getId();

    // get fields that need to be updated and normalize input
    out_ << "Fields available: title, start date, end date, active" << "\n";
    std::string toUpdate = getStringInput("Fields to update (comma separated)", " ");
    toUpdate = utils::stringLower(toUpdate);
    editFields = splitStringByComma(toUpdate);
    // get updated info and perform update
    for (auto& field : editFields) {
        // trim whitespace from strings
        field = utils::stringTrim(field);

        if (field == "title") {
            std::string newTitle = getStringInput("New title", " ");

            try {
                utils::validateTitle(newTitle);
                controller_.editTitle(id, newTitle);
                editTitle = true;
            } catch (const std::exception& e) {
                out_ << "Invalid string. Cannot update title." << "\n";
            }
        } else if (field == "start date" || field == "startdate") {
            std::chrono::year_month_day newStartDate = getDateInput("New start date", {});

            try {
                utils::validateDate(newStartDate);
                controller_.editStartDate(id, newStartDate);
                editStartDate = true;
            } catch (const std::exception& e) {
                out_ << "Invalid date. Cannot update start date." << "\n";
            }
        } else if (field == "end date" || field == "enddate") {
            std::chrono::year_month_day newEndDate = getDateInput("New end date", {});
            
            try {
                utils::validateDate(newEndDate);
                controller_.editEndDate(id, newEndDate);
                editEndDate = true;
            } catch (const std::exception& e) {
                out_ << "Invalid date. Cannot update end date." << "\n";
            }
        } else if (field == "active") {
            bool newActive = getBoolInput("Is this a current term? (yes/no)", true);
            controller_.editActive(id, newActive);
            editActive = true;
        }
    }

    // show new values to confirm
    if (editTitle || editStartDate || editEndDate || editActive) {
        out_ << "New values:" << "\n";

        if (editTitle) {
            out_ << "Title: " << selectedTerm_->get().getTitle() << "\n";
        }

        if (editStartDate || editEndDate) {
            out_ << "Duration: " << selectedTerm_->get().getStartDate() << " - " << selectedTerm_->get().getEndDate() << "\n";
        }

        if (editActive) {
            out_ << "Current? " << utils::boolToString(selectedTerm_->get().getActive()) << "\n";
        }
    }

    // reset selectedTerm to allow the selection of a new one
    selectedTerm_.reset();
}

// prompt the user for the title to select a term from the list
void CliView::promptSelectTerm() {
    out_ << "Here is a list of all terms:" << "\n";
    displayTermListInfo();

    out_ << "Enter the following information for the term you'd like to select: " << "\n";
    std::string title = getStringInput("Title", " ");

    try {
        selectedTerm_ = controller_.getTerm(title);
        out_ << "Term '" << title << "' was selected." << "\n";
    } catch (const std::exception& e) {
        out_ << "Term not found. No selection made." << "\n";
        selectedTerm_.reset();
    }
}

// prompt the user for the title to remove a term from the list
void CliView::promptRemoveTerm() {
    out_ << "Enter the following information for the term you'd like to remove: " << "\n";
    std::string title = getStringInput("Title", " ");

    // confirmation before removal
    bool confirm = getBoolInput("Are you sure you want to remove this term? (yes/no)", false);

    if (confirm) {
        controller_.removeTerm(title);
        out_ << "Term '" << title << "' was removed." << "\n";
    } else {
        out_ << "Operation cancelled. Term '" << title << "' was not removed." << "\n";
    }
}

// ask the user for a char input, using the default value in the case of an invalid input 
char CliView::getCharInput(const std::string &label, const char defaultVal) const {
    out_ << label << " [default: " << defaultVal << "]: ";
    
    std::string input{};
    std::getline(in_, input);

    if (input.empty() || utils::isOnlyWhitespace(input)) {
        return defaultVal;
    }

    if (input.size() == 1) {
        return input[0];
    }

    // error message for multiple character input
    out_ << "Invalid input, only one character allowed." << "\n";
    return '\0';
}

// ask the user for a string input, using the default value in the case of an invalid input 
std::string CliView::getStringInput(const std::string &label, const std::string& defaultVal) const {
    out_ << label << " [default: " << defaultVal << "]: ";
    
    std::string input{};
    std::getline(in_, input);

    if (input.empty() || utils::isOnlyWhitespace(input)) {
        return defaultVal;
    }

    return input;
}

// ask the user for a date input, using the default value in the case of an invalid input
std::chrono::year_month_day CliView::getDateInput(const std::string &label, const std::chrono::year_month_day defaultVal) const {
    out_ << label << ": ";

    std::string input{};
    std::getline(in_, input);

    if (input.empty() || utils::isOnlyWhitespace(input)) {
        return defaultVal;
    }

    try {
        return parseDate(input);
    } catch (const std::invalid_argument &e) {
        out_ << "Invalid date. Using default." << "\n";
        return defaultVal;
    }
}

// ask the user for a float input, using the default value in the case of an invalid input
float CliView::getFloatInput(const std::string &label, const float defaultVal) const {
    out_ << label << " [default: " << defaultVal << "]: ";

    std::string input{};
    std::getline(in_, input);

    if (input.empty() || utils::isOnlyWhitespace(input)) {
        return defaultVal;
    }

    try {
        return std::stof(input);
    } catch (const std::invalid_argument &e) {
        out_ << "Invalid float. Using default." << "\n";
        return defaultVal;
    } catch (const std::out_of_range &e) {
        out_ << "Value out of range. Using default." << "\n";
        return defaultVal;
    }
}

// ask the user for an int input, using the default value in the case of an invalid input
int CliView::getIntInput(const std::string &label, const int defaultVal) const {
    out_ << label << " [default: " << defaultVal << "]: ";

    std::string input{};
    std::getline(in_, input);

    if (input.empty() || utils::isOnlyWhitespace(input)) {
        return defaultVal;
    }

    try {
        return std::stoi(input);
    } catch (const std::invalid_argument &e) {
        out_ << "Invalid integer. Using default." << "\n";
        return defaultVal;
    } catch (const std::out_of_range &e) {
        out_ << "Value out of range. Using default." << "\n";
        return defaultVal;
    }
}

// ask the user for a bool input, using the default value in the case of an invalid input
bool CliView::getBoolInput(const std::string &label, const bool defaultVal) const {
    // convert defaultVal to string before printing label
    std::string defaultToString = utils::stringLower(utils::boolToString(defaultVal));
    out_ << label << " [default: " << defaultToString << "]: ";

    std::string input{};
    std::getline(in_, input);

    if (input.empty() || utils::isOnlyWhitespace(input)) {
        return defaultVal;
    }

    // convert string to lowercase
    input = utils::stringLower(input);

    // manually convert string to bool
    if (input == "true" || input == "1" || input == "yes" || input == "y") {
        return true;
    } else if (input == "false" || input == "0" || input == "no" || input == "n") {
        return false;
    } else {
        throw std::invalid_argument("Invalid boolean string.");
    }
}

CliView::CliView(TermController &controller, std::istream& is, std::ostream& os)
    : controller_{controller}, in_{is}, out_{os} {}

// starts the main CLI loop and handles user interaction for the program
void CliView::run() {
    char userInput{};
    MenuState state = MenuState::term;

    displayIntro();

    while (state != MenuState::exit) {
        while (state == MenuState::term) {
            displayTermMenu();
            userInput = getCharInput("Response", 'X');
            userInput = std::toupper(userInput);

            switch (userInput) {
                case 'A':
                    promptAddTerm();
                    break;
                case 'E':
                    // edit term
                    promptEditTerm();
                    break;
                case 'S':
                    // select term
                    promptSelectTerm();

                    if (selectedTerm_) {
                        state = MenuState::course;
                    }
                    
                    break;
                case 'R':
                    // remove term
                    promptRemoveTerm();
                    break;
                case 'X':
                    // exit
                    state = MenuState::exit;
                    break;
                default:
                    // invalid selection
                    out_ << "Invalid selection. Please try again." << "\n";
            }
        }

        while (state == MenuState::course) {
            displayCourseMenu();
            userInput = getCharInput("Response", 'X');
            userInput = std::toupper(userInput);

            switch (userInput) {
                case 'A':
                    // add course
                    out_ << "Add course placeholder" << "\n";
                    break;
                case 'E':
                    // edit course
                    out_ << "Edit course placeholder" << "\n";
                    break;
                case 'S':
                    // select course
                    out_ << "Select course placeholder" << "\n";
                    state = MenuState::assignment;
                    break;
                case 'R':
                    // remove course
                    out_ << "Remove course placeholder" << "\n";
                    break;
                case 'X':
                    // exit
                    state = MenuState::term;
                    break;
                default:
                    // invalid selection
                    out_ << "Invalid selection. Please try again." << "\n";
            }
        }

        while (state == MenuState::assignment) {
            displayAssignmentMenu();
            userInput = getCharInput("Response", 'X');
            userInput = std::toupper(userInput);

            switch (userInput) {
                case 'A':
                    // add assignment
                    out_ << "Add assignment placeholder" << "\n";
                    break;
                case 'E':
                    // edit assignment
                    out_ << "Edit assignment placeholder" << "\n";
                    break;
                case 'S':
                    // select assignment
                    out_ << "Select assignment placeholder" << "\n";
                    break;
                case 'R':
                    // remove assignment
                    out_ << "Remove assignment placeholder" << "\n";
                    break;
                case 'X':
                    // exit
                    state = MenuState::course;
                    break;
                default:
                    // invalid selection
                    out_ << "Invalid selection. Please try again." << "\n";
            }
        }
    }
}