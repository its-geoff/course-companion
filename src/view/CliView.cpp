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

// anonymous namespace for local scope; only this file can see these structs
namespace {
    // edit term result struct to replace multiple flags
    struct EditTermResult {
        bool titleRequested{false};
        bool titleUpdated{false};

        bool startDateRequested{false};
        bool startDateUpdated{false};

        bool endDateRequested{false};
        bool endDateUpdated{false};

        bool activeRequested{false};
        bool activeUpdated{false};

        bool anyRequested() const {
            return titleRequested || startDateRequested || endDateRequested || activeRequested;
        }

        bool datesRequested() const {
            return startDateRequested || endDateRequested;
        }

        bool datesUpdated() const {
            return startDateUpdated || endDateUpdated;
        }
    };

    // edit course result struct to replace multiple flags
    struct EditCourseResult {
        bool titleRequested{false};
        bool titleUpdated{false};
        
        bool descriptionRequested{false};
        bool descriptionUpdated{false};

        bool startDateRequested{false};
        bool startDateUpdated{false};

        bool endDateRequested{false};
        bool endDateUpdated{false};

        bool numCreditsRequested{false};
        bool numCreditsUpdated{false};

        bool activeRequested{false};
        bool activeUpdated{false};

        bool anyRequested() const {
            return titleRequested || descriptionRequested || startDateRequested 
                || endDateRequested || numCreditsRequested || activeRequested;
        }

        bool datesRequested() const {
            return startDateRequested || endDateRequested;
        }

        bool datesUpdated() const {
            return startDateUpdated || endDateUpdated;
        }
    };
}

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

    // check if there are terms in the list
    if (controller_.getTermList().size() > 0) {
        out_ << "[A] Add term" << "\n";
        out_ << "[E] Edit term" << "\n";
        out_ << "[S] Select term" << "\n";
        out_ << "[R] Remove term" << "\n";
        out_ << "[X] Exit program" << "\n";
    } else {
        out_ << "[A] Add term" << "\n";
        out_ << "[X] Exit program" << "\n";
    }
}

// prints a menu of choices for the user to select a Course
void CliView::displayCourseMenu() const {
    displayDelim();
    out_ << "Term: " << selectedTerm_->get().getTitle() << "\n";
    displayDelim();
    out_ << "                                            Course Menu                                            " << "\n";
    out_ << "                                 What would you like to do today?                                  " << "\n";

    // check if there are courses in the list
    if (selectedTerm_->get().getCourseList().size() > 0) {
            out_ << "[A] Add course" << "\n";
            out_ << "[E] Edit course" << "\n";
            out_ << "[S] Select course" << "\n";
            out_ << "[R] Remove course" << "\n";
            out_ << "[X] Exit to term menu" << "\n";
    } else {
            out_ << "[A] Add course" << "\n";
            out_ << "[X] Exit to term menu" << "\n";
    }
}

// prints a menu of choices for the user to select an Assignment
void CliView::displayAssignmentMenu() const {
    displayDelim();
    out_ << "Term: " << selectedTerm_->get().getTitle() << "\n";
    out_ << "Course: " << selectedCourse_->get().getTitle() << "\n";
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

// displays information about all courses from the selected term
void CliView::displayCourseListInfo() const {
    const std::unordered_map<std::string, Course>& courses = selectedTerm_->get().getCourseList();

    for (const auto& [id, course] : courses) {
        displaySecondaryDelim();
        course.printCourseInfo(out_);
        displaySecondaryDelim();
    }
}

// displays invalid selection output
void CliView::displayInvalidSelection() const {
    out_ << "Invalid selection. Please try again." << "\n";
}

// prompt the user for information to add a term to the list
void CliView::promptAddTerm() {
    bool titleEmpty{true};
    bool invalidStartDate{true};
    bool invalidEndDate{true};
    bool invalidActive{true};

    out_ << "Enter the following information for the term you'd like to add: " << "\n";

    std::string title{};
    while (titleEmpty) {
        title = getStringInput("Title", " ");

        if (utils::isOnlyWhitespace(title)) {
            out_ << "Invalid title. Title must be non-empty. Please try again." << "\n";
        } else {
            titleEmpty = false;
        }
    }
    
    std::chrono::year_month_day startDate{};
    constexpr std::chrono::year_month_day emptyDate{};
    while (invalidStartDate) {
        try {
            startDate = getDateInput("Start Date (YYYY-MM-DD)", {});
            
            // check if date is empty (default); if not, validate input
            if (startDate == emptyDate) {
                out_ << "Using default date." << "\n";
            } else {
                utils::validateDate(startDate);
            }

            invalidStartDate = false;
        } catch (const std::exception& e) {
            out_ << "Invalid start date. Start date must be a valid date. Please try again." << "\n";
        }
    }
    
    std::chrono::year_month_day endDate{};
    while (invalidEndDate) {
        try {
            endDate = getDateInput("End Date (YYYY-MM-DD)", {});

            // check if date is empty (default); if not, validate input
            if (endDate == emptyDate) {
                out_ << "Using default date." << "\n";
            } else {
                utils::validateDate(endDate);
            }

            invalidEndDate = false;
        } catch (const std::exception& e) {
            out_ << "Invalid end date. End date must be a valid date. Please try again." << "\n";
        }
    }

    bool active{};
    while (invalidActive) {
        try {
            active = getBoolInput("Current term? (yes/no)", true);
            invalidActive = false;
        } catch (const std::exception& e) {
            out_ << "Invalid active flag. Active flag must be a valid boolean. Please try again." << "\n";
        }
    }

    try {
        controller_.addTerm(title, startDate, endDate, active);
        out_ << "Term '" << title << "' successfully added." << "\n";
    } catch (const std::logic_error& e) {
        out_ << "A term with this title already exists. Please choose a new title." << "\n";
    } catch (const std::exception& e) {
        out_ << "An expected error occurred while adding the term." << "\n";
    }
}

// prompt the user for the information to edit a term in the current list
void CliView::promptEditTerm() {
    EditTermResult resultFlags;
    std::vector<std::string> editFields{};

    out_ << "Enter the following information for the term you'd like to edit: " << "\n";
    std::string title = getStringInput("Title", " ");

    try {
        selectedTerm_ = controller_.findTerm(title);
    } catch (const std::exception& e) {
        out_ << "Term not found. Operation cancelled." << "\n";
        return;
    }
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
            resultFlags.titleRequested = true;

            try {
                std::string newTitle = getStringInput("New title", " ");
                utils::validateTitle(newTitle);
                controller_.editTitle(id, newTitle);
                resultFlags.titleUpdated = true;
            } catch (const std::invalid_argument& e) {
                out_ << "Empty string. Cannot update title." << "\n";
            } catch (const std::logic_error& e) {
                out_ << "A term with this title already exists. Cannot update title." << "\n";
            }
        } else if (field == "start date" || field == "startdate") {
            resultFlags.startDateRequested = true;

            try {
                std::chrono::year_month_day newStartDate = getDateInput("New start date", {});
                utils::validateDate(newStartDate);
                controller_.editStartDate(id, newStartDate);
                resultFlags.startDateUpdated = true;
            } catch (const std::exception& e) {
                out_ << "Invalid date. Cannot update start date." << "\n";
            }
        } else if (field == "end date" || field == "enddate") {
            resultFlags.endDateRequested = true;

            try {
                std::chrono::year_month_day newEndDate = getDateInput("New end date", {});
                utils::validateDate(newEndDate);
                controller_.editEndDate(id, newEndDate);
                resultFlags.endDateUpdated = true;
            } catch (const std::exception& e) {
                out_ << "Invalid date. Cannot update end date." << "\n";
            }
        } else if (field == "active") {
            resultFlags.activeRequested = true;

            try {
                bool newActive = getBoolInput("Is this a current term? (yes/no)", true);
                controller_.editActive(id, newActive);
                resultFlags.activeUpdated = true;
            } catch (const std::exception& e) {
                out_ << "Invalid boolean. Cannot update active flag." << "\n";
            }
        }
    }

    // update result output
    if (resultFlags.anyRequested()) {
        out_ << "Update results:" << "\n";

        if (resultFlags.titleRequested) {
            if (resultFlags.titleUpdated) {
                out_ << "Title: " << selectedTerm_->get().getTitle() << "\n";
            } else {
                out_ << "Title: (unchanged)" << "\n";
            }
        }

        if (resultFlags.datesRequested()) {
            if (resultFlags.datesUpdated()) {
                out_ << "Duration: " << selectedTerm_->get().getStartDate() << " - " << selectedTerm_->get().getEndDate() << "\n";
            } else {
                out_ << "Duration: (unchanged)" << "\n";
            }
        }

        if (resultFlags.activeRequested) {
            if (resultFlags.activeUpdated) {
                out_ << "Current? " << utils::boolToString(selectedTerm_->get().getActive()) << "\n";
            } else {
                out_ << "Current? (unchanged)" << "\n";
            }
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
        controller_.selectTerm(title);
        selectedTerm_ = controller_.findTerm(title);
        out_ << "Term '" << title << "' was selected." << "\n";
    } catch (const std::exception& e) {
        out_ << "Term not found. No selection made." << "\n";
        selectedTerm_.reset();
    }
}

// prompt the user for the title to remove a term from the list
void CliView::promptRemoveTerm() {
    bool invalidBool{true};

    out_ << "Enter the following information for the term you'd like to remove: " << "\n";
    std::string title = getStringInput("Title", " ");

    // confirmation before removal
    bool confirm{false};
    while (invalidBool) {
        try {
            confirm = getBoolInput("Are you sure you want to remove this term? (yes/no)", false);
            invalidBool = false;
        } catch (const std::exception& e) {
            out_ << "Invalid response. Please try again." << "\n";
        }
    }

    if (confirm) {
        try {
            controller_.removeTerm(title);
            out_ << "Term '" << title << "' was removed." << "\n";
        } catch (const std::exception& e) {
            out_ << "Term not found. Operation cancelled." << "\n";
        }
    } else {
        out_ << "Operation cancelled. Term '" << title << "' was not removed." << "\n";
    }
}

// prompt the user for information to add a course into the list
void CliView::promptAddCourse() {
    bool titleEmpty{true};
    bool invalidStartDate{true};
    bool invalidEndDate{true};
    bool invalidNumCredits{true};
    bool invalidActive{true};

    out_ << "Enter the following information for the course you'd like to add: " << "\n";

    std::string title{};
    while (titleEmpty) {
        title = getStringInput("Title", " ");

        if (utils::isOnlyWhitespace(title)) {
            out_ << "Invalid title. Title must be non-empty. Please try again." << "\n";
        } else {
            titleEmpty = false;
        }
    }

    std::string description{};
    description = getStringInput("Description", " ");

    // empty description condition
    if (utils::isOnlyWhitespace(description)) {
        description = "";
    }

    std::chrono::year_month_day startDate{};
    constexpr std::chrono::year_month_day emptyDate{};
    while (invalidStartDate) {
        try {
            startDate = getDateInput("Start Date (YYYY-MM-DD)", {});
            
            // check if date is empty (default); if not, validate input
            if (startDate == emptyDate) {
                out_ << "Using default date." << "\n";
            } else {
                utils::validateDate(startDate);
            }

            invalidStartDate = false;
        } catch (const std::exception& e) {
            out_ << "Invalid start date. Start date must be a valid date. Please try again." << "\n";
        }
    }
    
    std::chrono::year_month_day endDate{};
    while (invalidEndDate) {
        try {
            endDate = getDateInput("End Date (YYYY-MM-DD)", {});

            // check if date is empty (default); if not, validate input
            if (endDate == emptyDate) {
                out_ << "Using default date." << "\n";
            } else {
                utils::validateDate(endDate);
            }

            invalidEndDate = false;
        } catch (const std::exception& e) {
            out_ << "Invalid end date. End date must be a valid date. Please try again." << "\n";
        }
    }

    int numCredits{};
    while (invalidNumCredits) {
        numCredits = getIntInput("Number of credits", 3);
        
        if (numCredits < 0) {
            out_ << "Invalid number of credits. Number of credits must be greater than or equal to 0. Please try again." << "\n";
        } else {
            invalidNumCredits = false;
        }
    }

    bool active{};
    while (invalidActive) {
        try {
            active = getBoolInput("Current course? (yes/no)", true);
            invalidActive = false;
        } catch (const std::exception& e) {
            out_ << "Invalid active flag. Active flag must be a valid boolean. Please try again." << "\n";
        }
    }

    try {
        CourseController& courseController = controller_.getCourseController();
        courseController.addCourse(title, description, startDate, endDate, numCredits, active);
    } catch (const std::logic_error& e) {
        out_ << "A course with this title already exists. Please choose a new title." << "\n";
    } catch (const std::exception& e) {
        out_ << "An unexpected error occurred while adding the course." << "\n";
    }
}

// prompt the user for information to edit a course in the current list
void CliView::promptEditCourse() {
    EditCourseResult resultFlags;
    std::vector<std::string> editFields{};
    CourseController& courseController = controller_.getCourseController();

    out_ << "Enter the following information for the course you'd like to edit: " << "\n";
    std::string title = getStringInput("Title", " ");

    try {
        selectedCourse_ = courseController.findCourse(title);
    } catch (const std::exception& e) {
        out_ << "Course not found. Operation cancelled." << "\n";
        return;
    }
    std::string id = selectedCourse_->get().getId();

    // get fields that need to be updated and normalize input
    out_ << "Fields available: title, description, start date, end date, number of credits, active" << "\n";
    std::string toUpdate = getStringInput("Fields to update (comma separated)", " ");
    toUpdate = utils::stringLower(toUpdate);
    editFields = splitStringByComma(toUpdate);

    // get updated info and perform update
    for (auto& field : editFields) {
        // trim whitespace from strings
        field = utils::stringTrim(field);

        if (field == "title") {
            resultFlags.titleRequested = true;

            try {
                std::string newTitle = getStringInput("New title", " ");
                utils::validateTitle(newTitle);
                courseController.editTitle(id, newTitle);
                resultFlags.titleUpdated = true;
            } catch (const std::invalid_argument& e) {
                out_ << "Empty string. Cannot update title." << "\n";
            } catch (const std::logic_error& e) {
                out_ << "A course with this title already exists. Cannot update title." << "\n";
            }
        } else if (field == "description") {
            resultFlags.descriptionRequested = true;

            std::string oldDescription = selectedCourse_->get().getDescription();
            std::string newDescription = getStringInput("New description", " ");
            courseController.editDescription(id, newDescription);

            // only set descriptionUpdated if old and new descriptions are not both whitespace
            if (!(utils::isOnlyWhitespace(oldDescription)) || !(utils::isOnlyWhitespace(newDescription))) {
                resultFlags.descriptionUpdated = true;
            }
        } else if (field == "start date" || field == "startdate") {
            resultFlags.startDateRequested = true;

            try {
                std::chrono::year_month_day newStartDate = getDateInput("New start date", {});
                utils::validateDate(newStartDate);
                courseController.editStartDate(id, newStartDate);
                resultFlags.startDateUpdated = true;
            } catch (const std::exception& e) {
                out_ << "Invalid date. Cannot update start date." << "\n";
            }
        } else if (field == "end date" || field == "enddate") {
            resultFlags.endDateRequested = true;

            try {
                std::chrono::year_month_day newEndDate = getDateInput("New end date", {});
                utils::validateDate(newEndDate);
                courseController.editEndDate(id, newEndDate);
                resultFlags.endDateUpdated = true;
            } catch (const std::exception& e) {
                out_ << "Invalid date. Cannot update end date." << "\n";
            }
        } else if (field == "number of credits" || field == "numberofcredits" || field == "numcredits") {
            resultFlags.numCreditsRequested = true;

            int newNumCredits = getIntInput("New number of credits", 3);

            if (newNumCredits < 0) {
                out_ << "Number of credits must be greater than or equal to 0. Cannot update number of credits." << "\n";
            } else {
                courseController.editNumCredits(id, newNumCredits);
                resultFlags.numCreditsUpdated = true;
            }
        } else if (field == "active") {
            resultFlags.activeRequested = true;

            try {
                bool newActive = getBoolInput("Is this a current course? (yes/no)", true);
                courseController.editActive(id, newActive);
                resultFlags.activeUpdated = true;
            } catch (const std::exception& e) {
                out_ << "Invalid boolean. Cannot update active flag." << "\n";
            }
        }
    }

    if (resultFlags.anyRequested()) {
        out_ << "Update results:" << "\n";

        if (resultFlags.titleRequested) {
            if (resultFlags.titleUpdated) {
                out_ << "Title: " << selectedCourse_->get().getTitle() << "\n";
            } else {
                out_ << "Title: (unchanged)" << "\n";
            }
        }

        if (resultFlags.descriptionRequested) {
            if (resultFlags.descriptionUpdated) {
                out_ << "Description: " << selectedCourse_->get().getDescription() << "\n";
            } else {
                out_ << "Description: (unchanged)" << "\n";
            }
        }

        if (resultFlags.datesRequested()) {
            if (resultFlags.datesUpdated()) {
                out_ << "Duration: " << selectedCourse_->get().getStartDate() << " - " << selectedCourse_->get().getEndDate() << "\n";
            } else {
                out_ << "Duration: (unchanged)" << "\n";
            }
        }

        if (resultFlags.numCreditsRequested) {
            if (resultFlags.numCreditsUpdated) {
                out_ << "Number of credits: " << selectedCourse_->get().getNumCredits() << "\n";
            } else {
                out_ << "Number of credits: (unchanged)" << "\n";
            }
        }

        if (resultFlags.activeRequested) {
            if (resultFlags.activeUpdated) {
                out_ << "Current? " << utils::boolToString(selectedCourse_->get().getActive()) << "\n";
            } else {
                out_ << "Current? (unchanged)" << "\n";
            }
        }
    }

    // reset selectedCourse to allow the selection of a new one
    selectedCourse_.reset();
}

// prompt the user for the title to select a course from the list
void CliView::promptSelectCourse() {
    CourseController& courseController = controller_.getCourseController();

    out_ << "Here is a list of all courses:" << "\n";
    displayCourseListInfo();

    out_ << "Enter the following information for the course you'd like to select: " << "\n";
    std::string title = getStringInput("Title", " ");

    try {
        selectedCourse_ = courseController.findCourse(title);
        out_ << "Course '" << title << "' was selected." << "\n";
    } catch (const std::exception& e) {
        out_ << "Course not found. No selection made." << "\n";
        selectedCourse_.reset();
    }
}

// prompt the user for the title to remove a course from the list
void CliView::promptRemoveCourse() {
    bool invalidBool{true};
    CourseController& courseController = controller_.getCourseController();

    out_ << "Enter the following information for the course you'd like to remove: " << "\n";
    std::string title = getStringInput("Title", " ");

    // confirmation before removal
    bool confirm{false};
    while (invalidBool) {
        try {
            confirm = getBoolInput("Are you sure you want to remove this course? (yes/no)", false);
            invalidBool = false;
        } catch (const std::exception& e) {
            out_ << "Invalid response. Please try again." << "\n";
        }
    }

    if (confirm) {
        try {
            courseController.removeCourse(title);
            out_ << "Course '" << title << "' was removed." << "\n";
        } catch (const std::exception& e) {
            out_ << "Course not found. Operation cancelled." << "\n";
        }
    } else {
        out_ << "Operation cancelled. Course '" << title << "' was not removed." << "\n";
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
    } catch (const std::exception& e) {
        throw std::invalid_argument("Invalid date format. Expected YYYY-MM-DD.");
    }
}

// ask the user for a float input, using the default value in the case of an invalid input
float CliView::getFloatInput(const std::string &label, const float defaultVal) const {
    out_ << label << " [default: " << defaultVal << "]: ";

    std::string input{};
    std::getline(in_, input);

    if (input.empty() || utils::isOnlyWhitespace(input)) {
        out_ << "No input. Using default." << "\n";
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
        out_ << "No input. Using default." << "\n";
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
        out_ << "No input. Using default." << "\n";
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
                    if (controller_.getTermList().size() > 0) {
                        promptEditTerm();
                    } else {
                        displayInvalidSelection();
                    }

                    break;
                case 'S':
                    // select term
                    if (controller_.getTermList().size() > 0) {
                        promptSelectTerm();

                        if (selectedTerm_) {
                            state = MenuState::course;
                        }
                    } else {
                        displayInvalidSelection();
                    }
                    
                    break;
                case 'R':
                    // remove term
                    if (controller_.getTermList().size() > 0) {
                        promptRemoveTerm();
                    } else {
                        displayInvalidSelection();
                    }

                    break;
                case 'X':
                    // exit
                    state = MenuState::exit;
                    break;
                default:
                    // invalid selection
                    displayInvalidSelection();
                    break;
                }
        }

        while (state == MenuState::course) {
            displayCourseMenu();
            userInput = getCharInput("Response", 'X');
            userInput = std::toupper(userInput);

            switch (userInput) {
                case 'A':
                    // add course
                    promptAddCourse();
                    break;
                case 'E':
                    // edit course
                    if (selectedTerm_->get().getCourseList().size() > 0) {
                        promptEditCourse();
                    } else {
                        displayInvalidSelection();
                    }

                    break;
                case 'S':
                    // select course
                    if (selectedTerm_->get().getCourseList().size() > 0) {
                        promptSelectCourse();

                        if (selectedCourse_) {
                            state = MenuState::assignment;
                        }
                    } else {
                        displayInvalidSelection();
                    }

                    break;
                case 'R':
                    // remove course
                    if (selectedTerm_->get().getCourseList().size() > 0) {
                        promptRemoveCourse();
                    } else {
                        displayInvalidSelection();
                    }

                    break;
                case 'X':
                    // exit
                    selectedTerm_.reset();
                    state = MenuState::term;
                    break;
                default:
                    // invalid selection
                    displayInvalidSelection();
                    break;
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
                    selectedCourse_.reset();
                    state = MenuState::course;
                    break;
                default:
                    // invalid selection
                    displayInvalidSelection();
                    break;
                }
        }
    }
}