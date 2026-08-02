#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QDate>
#include "view/qt/TermView.hpp"
#include "controller/TermController.hpp"

using namespace std::chrono_literals;

// test fixture for class TermView
class TermViewTest : public testing::Test {
    protected:
        TermController controller{};
        TermView view{controller};
};


// ====================================
// FUNCTION SMOKE TESTS
// ====================================

TEST_F(TermViewTest, SubmitAddTermMutatesController) {
    QSignalSpy spy(&controller, &TermController::dataChanged);

    view.submitAddTerm("Fall 2025", QDate(2025, 8, 15), QDate(2025, 12, 17), true);

    ASSERT_EQ(spy.count(), 1);
    ASSERT_EQ(controller.getTermList().size(), 1);

    const Term& term = controller.findTerm("Fall 2025");
    ASSERT_EQ(term.getTitle(), "Fall 2025");
    ASSERT_TRUE(term.getActive());
}

TEST_F(TermViewTest, SubmitAddTermMultipleCallsAccumulate) {
    QSignalSpy spy(&controller, &TermController::dataChanged);

    view.submitAddTerm("Fall 2025", QDate(2025, 8, 15), QDate(2025, 12, 17), false);
    view.submitAddTerm("Spring 2026", QDate(2026, 1, 2), QDate(2026, 5, 24), true);

    ASSERT_EQ(spy.count(), 2);
    ASSERT_EQ(controller.getTermList().size(), 2);
}


// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST_F(TermViewTest, SubmitAddTermDuplicateTitleDoesNotMutate) {
    view.submitAddTerm("Fall 2025", QDate(2025, 8, 15), QDate(2025, 12, 17), true);

    QSignalSpy spy(&controller, &TermController::dataChanged);
    view.submitAddTerm("Fall 2025", QDate(2026, 1, 2), QDate(2026, 5, 24), false);

    ASSERT_EQ(spy.count(), 0);
    ASSERT_EQ(controller.getTermList().size(), 1);
}

TEST_F(TermViewTest, SubmitAddTermEmptyTitleDoesNotMutate) {
    QSignalSpy spy(&controller, &TermController::dataChanged);

    view.submitAddTerm("", QDate(2025, 8, 15), QDate(2025, 12, 17), true);

    ASSERT_EQ(spy.count(), 0);
    ASSERT_EQ(controller.getTermList().size(), 0);
}