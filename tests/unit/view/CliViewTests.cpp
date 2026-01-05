#include <gtest/gtest.h>
#include <sstream>      // diverts output from terminal to separate stream
#include <regex>        // regular expression matching for UUIDs
#include "view/CliView.hpp"
#include "controller/MockController.hpp"    // define controller
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
    std::istringstream input("x\n");
    std::ostringstream output;

    MockController controller;
    CliView view(controller, input, output);

    view.run();

    // check if "Welcome" is printed
    EXPECT_TRUE(output.str().find("Welcome") != std::string::npos);
}

// ====================================
// INITIALIZATION EDGE CASES
// ====================================

TEST(CliViewTest, DefaultOstreamCout) {
    MockController controller;

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
    MockController controller;

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
    MockController controller;
    std::istringstream simInput("\n");
    std::ostringstream os;

    // create scope for view
    {
        CliView view(controller, simInput, os);
        view.run();
    }

    // verify that intro message was sent and there is no invalid selection
    ASSERT_NE(os.str().find("Welcome to Course Companion!"), std::string::npos);
    ASSERT_EQ(os.str().find("Invalid selection."), std::string::npos);
}

TEST(CliViewTest, InvalidInput) {
    MockController controller;
    std::istringstream simInput("I\n");
    std::ostringstream os;

    // create scope for view
    {
        CliView view(controller, simInput, os);
        view.run();
    }

    // verify that intro message and invalid selection were sent
    ASSERT_NE(os.str().find("Welcome to Course Companion!"), std::string::npos);
    ASSERT_NE(os.str().find("Invalid selection."), std::string::npos);
}

TEST(CliViewTest, LowercaseInput) {
    MockController controller;
    std::istringstream simInput("a\n");
    std::ostringstream os;

    // create scope for view
    {
        CliView view(controller, simInput, os);
        // throw invalid argument since empty title is given
        ASSERT_THROW(view.run(), std::invalid_argument);
    }

    // verify that intro message and add term prompt were sent
    ASSERT_NE(os.str().find("Welcome to Course Companion!"), std::string::npos);
    ASSERT_NE(os.str().find("Enter the following information"), std::string::npos);
}

TEST(CliViewTest, LongInput) {
    MockController controller;
    std::istringstream simInput("Avatar\n");
    std::ostringstream os;

    // create scope for view
    {
        CliView view(controller, simInput, os);
        view.run();
    }

    // verify that intro message and invalid char input message was sent
    ASSERT_NE(os.str().find("Welcome to Course Companion!"), std::string::npos);
    ASSERT_NE(os.str().find("Invalid input"), std::string::npos);
}