#include <gtest/gtest.h>
#include <sstream>      // diverts output from terminal to separate stream
#include <regex>        // regular expression matching for UUIDs
#include "view/CliView.hpp"
#include "controller/TermController.hpp"    // define controller
#include "utils/utils.hpp"

using namespace std::chrono_literals;

// ====================================
// INITIALIZATION SMOKE TESTS
// ====================================

TEST(CliViewTest, FullConstructorInitialization) {
    TermController controller;
    std::istringstream is;
    std::ostringstream os;
    CliView view2{controller, is, os};
    SUCCEED();
}

// ====================================
// FUNCTION SMOKE TESTS
// ====================================

TEST(CliViewTest, UserExitsImmediately) {
    std::istringstream input("X\n");
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);

    view.run();

    // check if "Welcome" is printed
    ASSERT_TRUE(output.str().find("Welcome") != std::string::npos);
}

TEST(CliViewTest, AddTerm) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);
    
    // check that Term was added to the list
    const Term &selectedTerm = controller.getTerm("Spring 2025");
    ASSERT_EQ(selectedTerm.getTitle(), "Spring 2025");
    ASSERT_EQ(selectedTerm.getStartDate(), std::chrono::year_month_day{2025y/1/10});
    ASSERT_EQ(selectedTerm.getEndDate(), std::chrono::year_month_day{2025y/5/23});
    ASSERT_TRUE(selectedTerm.getActive());

    // check user output for both the prompt and the success message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Enter the following information") != std::string::npos);
    ASSERT_TRUE(userOut.find("successfully added") != std::string::npos);
}

TEST(CliViewTest, EditTerm) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // edit term
        "E\n"
        "Spring 2025\n"
        "title, start date, end date, active\n"
        "Fall 2025\n"
        "2025-8-2\n"
        "2025-12-11\n"
        "no\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check that info was updated in list
    const Term &selectedTerm = controller.getTerm("Fall 2025");
    ASSERT_EQ(selectedTerm.getTitle(), "Fall 2025");
    ASSERT_EQ(selectedTerm.getStartDate(), std::chrono::year_month_day{2025y/8/2});
    ASSERT_EQ(selectedTerm.getEndDate(), std::chrono::year_month_day{2025y / 12 / 11});
    ASSERT_FALSE(selectedTerm.getActive());

    // check user output for both the prompt and the success message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Enter the following information") != std::string::npos);
    ASSERT_TRUE(userOut.find("Update results") != std::string::npos);
}

TEST(CliViewTest, SelectTerm) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // select term
        "S\n"
        "Spring 2025\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check user output for both the prompt and the success message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Enter the following information") != std::string::npos);
    ASSERT_TRUE(userOut.find("was selected") != std::string::npos);
}

TEST(CliViewTest, RemoveTerm) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        "A\n"
        "Fall 2025\n"
        "2025-8-2\n"
        "2025-12-11\n"
        "no\n"
        // remove term
        "R\n"
        "Spring 2025\n"
        "yes\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is 1
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check that termList does not include removed Term
    ASSERT_THROW(controller.getTerm("Spring 2025"), std::out_of_range);

    // check user output for both the prompt and the success message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Enter the following information") != std::string::npos);
    ASSERT_TRUE(userOut.find("was removed") != std::string::npos);
}

// ====================================
// INITIALIZATION EDGE CASES
// ====================================

TEST(CliViewTest, DefaultOstreamCout) {
    TermController controller;

    std::ostringstream captured;
    // redirect output buffer
    auto *oldBuf = std::cout.rdbuf(captured.rdbuf());
    
    // create scope for view
    {
        CliView view(controller);
        view.run();
    }

    std::cout.rdbuf(oldBuf);  // restore old buffer

    // check that something was output to cout
    EXPECT_FALSE(captured.str().empty());
}

TEST(CliViewTest, DefaultIstreamCin) {
    TermController controller;

    std::istringstream fakeInput("x\n");
    // redirect input buffer
    auto *oldCinBuf = std::cin.rdbuf(fakeInput.rdbuf());

    std::ostringstream fakeOutput;
    auto* oldCoutBuf = std::cout.rdbuf(fakeOutput.rdbuf());

    // create scope for view
    {
        CliView view(controller);
        view.run();
    }

    std::cin.rdbuf(oldCinBuf);  // restore old buffer
    std::cout.rdbuf(oldCoutBuf);

    // check that input didn't hang
    SUCCEED();
}

// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST(CliViewTest, EmptyInput) {
    TermController controller;
    std::istringstream simInput(
        // empty input
        "\n"
        // exit
        "X\n"
    );
    std::ostringstream os;

    CliView view(controller, simInput, os);
    view.run();

    // verify that intro message was sent and there is no invalid selection
    ASSERT_TRUE(os.str().find("Welcome to Course Companion") != std::string::npos);
    ASSERT_EQ(os.str().find("Invalid selection"), std::string::npos);
}

TEST(CliViewTest, InvalidInput) {
    TermController controller;
    std::istringstream simInput(
        // invalid input
        "I\n"
        // exit
        "X\n"
    );
    std::ostringstream os;

    CliView view(controller, simInput, os);
    view.run();

    // verify that intro message and invalid selection were sent
    ASSERT_TRUE(os.str().find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(os.str().find("Invalid selection") != std::string::npos);
}

TEST(CliViewTest, LowercaseInput) {
    TermController controller;
    std::istringstream simInput(
        // lowercase input, add term
        "a\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // exit
        "X\n"
    );
    std::ostringstream os;

    CliView view(controller, simInput, os);
    view.run();

    // verify that intro message and add term prompt were sent
    ASSERT_TRUE(os.str().find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(os.str().find("Enter the following information") != std::string::npos);
}

TEST(CliViewTest, LongInput) {
    TermController controller;
    std::istringstream simInput(
        // long input
        "Avatar\n"
        // exit
        "X\n"
    );
    std::ostringstream os;

    CliView view(controller, simInput, os);
    view.run();

    // verify that intro message and invalid char input message was sent
    ASSERT_TRUE(os.str().find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(os.str().find("Invalid input") != std::string::npos);
}

TEST(CliViewTest, TermMenuInvalidSelection) {
    std::istringstream input(
        // try to remove term
        "R\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check for intro and invalid selection message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid selection") != std::string::npos);
}

TEST(CliViewTest, AddTermTitleEmpty) {
    std::istringstream input(
        // add term
        "A\n"
        "\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid title") != std::string::npos);
    ASSERT_TRUE(userOut.find("must be non-empty") != std::string::npos);
}

TEST(CliViewTest, AddTermStartDateEmpty) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid start date") != std::string::npos);
    ASSERT_TRUE(userOut.find("Start date must be a valid date") != std::string::npos);
}

TEST(CliViewTest, AddTermStartDateWrongFormat) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2-12-2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid start date") != std::string::npos);
    ASSERT_TRUE(userOut.find("Start date must be a valid date") != std::string::npos);
}

TEST(CliViewTest, AddTermStartDateNonexistent) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-2-31\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid start date") != std::string::npos);
    ASSERT_TRUE(userOut.find("Start date must be a valid date") != std::string::npos);
}

TEST(CliViewTest, AddTermEndDateEmpty) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-22\n"
        "\n"
        "2025-5-23\n"
        "yes\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid end date") != std::string::npos);
    ASSERT_TRUE(userOut.find("End date must be a valid date") != std::string::npos);
}

TEST(CliViewTest, AddTermEndDateWrongFormat) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-22\n"
        "5-28-2025\n"
        "2025-5-23\n"
        "yes\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid end date") != std::string::npos);
    ASSERT_TRUE(userOut.find("End date must be a valid date") != std::string::npos);
}

TEST(CliViewTest, AddTermEndDateNonexistent) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-22\n"
        "2025-5-40\n"
        "2025-5-23\n"
        "yes\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid end date") != std::string::npos);
    ASSERT_TRUE(userOut.find("End date must be a valid date") != std::string::npos);
}

TEST(CliViewTest, AddTermActiveEmpty) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-22\n"
        "2025-5-18\n"
        "\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check for intro and successful add message; default active is used
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("successfully added") != std::string::npos);
}

TEST(CliViewTest, AddTermActiveInvalid) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-22\n"
        "2025-5-18\n"
        "maybe\n"
        "yes\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid active flag") != std::string::npos);
    ASSERT_TRUE(userOut.find("must be a valid boolean") != std::string::npos);
}

TEST(CliViewTest, AddTermAlreadyExists) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-22\n"
        "2025-5-18\n"
        "yes\n"
        "A\n"
        "Spring 2025\n"
        "2025-1-22\n"
        "2025-5-18\n"
        "yes\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("with this title already exists") != std::string::npos);
    ASSERT_TRUE(userOut.find("choose a new title") != std::string::npos);
}

TEST(CliViewTest, EditTermNotFound) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // edit term
        "E\n"
        "Fall 2025\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check for intro and term not found message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Term not found") != std::string::npos);
    ASSERT_TRUE(userOut.find("Operation cancelled") != std::string::npos);
}

TEST(CliViewTest, EditTermNoEditFields) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // edit term
        "E\n"
        "Spring 2025\n"
        "\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check for intro and fields to update message (no error should be thrown)
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Fields available") != std::string::npos);
    ASSERT_TRUE(userOut.find("Fields to update (comma separated)") != std::string::npos);
}

TEST(CliViewTest, EditTermNewTitleEmpty) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // edit term
        "E\n"
        "Spring 2025\n"
        "title\n"
        "\n"
        "Fall 2025\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid string") != std::string::npos);
    ASSERT_TRUE(userOut.find("Cannot update title") != std::string::npos);
}

TEST(CliViewTest, EditTermNewTitleAlreadyExists) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        "A\n"
        "Fall 2025\n"
        "2025-8-2\n"
        "2025-12-11\n"
        "no\n"
        // edit term
        "E\n"
        "Spring 2025\n"
        "title\n"
        "Spring 2025\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 2);

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("with this title already exists") != std::string::npos);
    ASSERT_TRUE(userOut.find("Cannot update title") != std::string::npos);
}

TEST(CliViewTest, EditTermStartDateEmpty) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // edit term
        "E\n"
        "Spring 2025\n"
        "start date\n"
        "Fall 2025\n"
        "\n"
        "2025-8-16\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid date") != std::string::npos);
    ASSERT_TRUE(userOut.find("Cannot update start date") != std::string::npos);
}

TEST(CliViewTest, EditTermStartDateWrongFormat) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // edit term
        "E\n"
        "Spring 2025\n"
        "start date\n"
        "Fall 2025\n"
        "1-10-2025\n"
        "2025-8-10\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid date") != std::string::npos);
    ASSERT_TRUE(userOut.find("Cannot update start date") != std::string::npos);
}

TEST(CliViewTest, EditTermStartDateNonexistent) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // edit term
        "E\n"
        "Spring 2025\n"
        "start date\n"
        "Fall 2025\n"
        "2025-2-32\n"
        "2025-8-1\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid date") != std::string::npos);
    ASSERT_TRUE(userOut.find("Cannot update start date") != std::string::npos);
}

TEST(CliViewTest, EditTermEndDateEmpty) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // edit term
        "E\n"
        "Spring 2025\n"
        "end date\n"
        "Fall 2025\n"
        "2025-1-22\n"
        "\n"
        "2025-12-1\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid date") != std::string::npos);
    ASSERT_TRUE(userOut.find("Cannot update end date") != std::string::npos);
}

TEST(CliViewTest, EditTermEndDateWrongFormat) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // edit term
        "E\n"
        "Spring 2025\n"
        "end date\n"
        "Fall 2025\n"
        "2025-1-22\n"
        "5-20-2025\n"
        "2025-11-28\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid date") != std::string::npos);
    ASSERT_TRUE(userOut.find("Cannot update end date") != std::string::npos);
}

TEST(CliViewTest, EditTermEndDateNonexistent) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // edit term
        "E\n"
        "Spring 2025\n"
        "end date\n"
        "Fall 2025\n"
        "2025-1-22\n"
        "2025-5-33\n"
        "2025-12-4\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid date") != std::string::npos);
    ASSERT_TRUE(userOut.find("Cannot update end date") != std::string::npos);
}

TEST(CliViewTest, EditTermActiveEmpty) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // edit term
        "E\n"
        "Spring 2025\n"
        "active\n"
        "Fall 2025\n"
        "2025-1-22\n"
        "2025-5-23\n"
        "\n"
        "no\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid boolean") != std::string::npos);
    ASSERT_TRUE(userOut.find("Cannot update active flag") != std::string::npos);
}

TEST(CliViewTest, EditTermActiveInvalid) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // edit term
        "E\n"
        "Spring 2025\n"
        "active\n"
        "Fall 2025\n"
        "2025-1-22\n"
        "2025-5-23\n"
        "maybe\n"
        "no\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check for intro and invalid input message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid boolean") != std::string::npos);
    ASSERT_TRUE(userOut.find("Cannot update active flag") != std::string::npos);
}

TEST(CliViewTest, SelectTermNotFound) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        // select term
        "S\n"
        "Fall 2025\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check for intro and term not found message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Term not found") != std::string::npos);
    ASSERT_TRUE(userOut.find("No selection made") != std::string::npos);
}

TEST(CliViewTest, RemoveTermNotFound) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        "A\n"
        "Fall 2025\n"
        "2025-8-2\n"
        "2025-12-11\n"
        "no\n"
        // remove term
        "R\n"
        "Spring 2026\n"
        "yes\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is 2 since Term was not removed
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 2);

    // check for intro and term not found message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Term not found") != std::string::npos);
    ASSERT_TRUE(userOut.find("Operation cancelled") != std::string::npos);
}

TEST(CliViewTest, RemoveTermInvalidBool) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        "A\n"
        "Fall 2025\n"
        "2025-8-2\n"
        "2025-12-11\n"
        "no\n"
        // remove term
        "R\n"
        "Spring 2025\n"
        "maybe\n"
        "yes\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is not 0
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check for intro and invalid response message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Invalid response") != std::string::npos);
    ASSERT_TRUE(userOut.find("Please try again") != std::string::npos);
}

TEST(CliViewTest, RemoveTermNotConfirmed) {
    std::istringstream input(
        // add term
        "A\n"
        "Spring 2025\n"
        "2025-1-10\n"
        "2025-5-23\n"
        "yes\n"
        "A\n"
        "Fall 2025\n"
        "2025-8-2\n"
        "2025-12-11\n"
        "no\n"
        // remove term
        "R\n"
        "Spring 2025\n"
        "no\n"
        // exit
        "X\n"
    );
    std::ostringstream output;

    TermController controller;
    CliView view(controller, input, output);
    view.run();

    // check that termList size is 2 since Term was not removed
    const std::unordered_map<std::string, Term>& listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 2);

    // check for intro and operation cancelled message
    const std::string userOut = output.str();
    ASSERT_TRUE(userOut.find("Welcome to Course Companion") != std::string::npos);
    ASSERT_TRUE(userOut.find("Operation cancelled") != std::string::npos);
    ASSERT_TRUE(userOut.find("not removed") != std::string::npos);
}