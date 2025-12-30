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

std::chrono::year_month_day CliView::parseDate(const std::string &input) const {
    int y, m, d;

    if (sscanf(input.c_str(), "%d-%d-%d", &y, &m, &d) == 3) {
        return std::chrono::year{y}/std::chrono::month{static_cast<unsigned>(m)}/std::chrono::day{static_cast<unsigned>(d)};
    }

    throw std::invalid_argument("Invalid date format. Expected YYYY-MM-DD.");
}

// prints an introduction to the application
void CliView::displayIntro() const {
    displayDelim();
    out_ << "               Welcome to Course Companion!                " << "\n";
    out_ << "           It's your personal academic sidekick.           " << "\n";
}

// prints a menu of choices for the user to select a Term
void CliView::displayTermMenu() const {
    displayDelim();
    out_ << "             What would you like to do today?              " << "\n";
    out_ << "[A] Add term" << "\n";
    out_ << "[E] Edit term" << "\n";
    out_ << "[F] Find term" << "\n";
    out_ << "[R] Remove term" << "\n";
    out_ << "[X] Exit program" << "\n";
}

// prints a menu of choices for the user to select a Course
void CliView::displayCourseMenu() const {
    displayDelim();
    out_ << "Term: " << "\n";
    displayDelim();
    out_ << "             What would you like to do today?              " << "\n";
    out_ << "[A] Add course" << "\n";
    out_ << "[E] Edit course" << "\n";
    out_ << "[F] Find course" << "\n";
    out_ << "[R] Remove course" << "\n";
    out_ << "[X] Exit to term menu" << "\n";
}

// prints a menu of choices for the user to select an Assignment
void CliView::displayAssignmentMenu() const {
    displayDelim();
    out_ << "Term: " << "\n";
    out_ << "Course: " << "\n";
    displayDelim();
    out_ << "             What would you like to do today?              " << "\n";
    out_ << "[A] Add assignment" << "\n";
    out_ << "[E] Edit assignment" << "\n";
    out_ << "[F] Find assignment" << "\n";
    out_ << "[R] Remove assignment" << "\n";
    out_ << "[X] Exit to course menu" << "\n";
}

// prints a delimiter line to separate different sections of information
void CliView::displayDelim() const {
    out_ << "===========================================================" << "\n";
}

// ask the user for a char input, using the default value in the case of an invalid input 
char CliView::getCharInput(const std::string &label, const char defaultVal) const {
    out_ << label << " [default: " << defaultVal << "]: ";
    
    std::string input{};
    std::getline(in_, input);

    if (input.empty() || utils::isOnlyWhitespace(input)) {
        return defaultVal;
    }

    return input[0];
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
    out_ << label << " [default: " << defaultVal << "]: ";

    std::string input{};
    std::getline(in_, input);

    if (input.empty() || utils::isOnlyWhitespace(input)) {
        return defaultVal;
    }

    try {
        return parseDate(input);
    } catch (const std::invalid_argument& e) {
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

    return std::stof(input);
}

// ask the user for an int input, using the default value in the case of an invalid input
int CliView::getIntInput(const std::string &label, const int defaultVal) const {
    out_ << label << " [default: " << defaultVal << "]: ";

    std::string input{};
    std::getline(in_, input);

    if (input.empty() || utils::isOnlyWhitespace(input)) {
        return defaultVal;
    }

    return std::stoi(input);
}

// ask the user for a bool input, using the default value in the case of an invalid input
bool CliView::getBoolInput(const std::string &label, const bool defaultVal) const {
    out_ << label << " [default: " << defaultVal << "]: ";

    std::string input{};
    std::getline(in_, input);

    if (input.empty() || utils::isOnlyWhitespace(input)) {
        return defaultVal;
    }

    // convert string to lowercase
    std::transform(input.begin(), input.end(), input.begin(),
        [](unsigned char c){ return std::tolower(c); });

    // manually convert string to bool
    if (input == "true" || input == "1" || input == "yes" || input == "y") {
        return true;
    } else if (input == "false" || input == "0" || input == "no" || input == "n") {
        return false;
    } else {
        throw std::invalid_argument("Invalid boolean string: " + input);
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
                    // add term
                    out_ << "Add term placeholder" << "\n";
                    break;
                case 'E':
                    // edit term
                    out_ << "Edit term placeholder" << "\n";
                    break;
                case 'F':
                    // find term
                    out_ << "Find term placeholder" << "\n";
                    break;
                case 'R':
                    // remove term
                    out_ << "Remove term placeholder" << "\n";
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
    }
}