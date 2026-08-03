#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QDate>
#include <QTimer>
#include <QApplication>
#include "view/qt/TermView.hpp"
#include "controller/TermController.hpp"

using namespace std::chrono_literals;

// test fixture for class TermView
class TermViewTests : public testing::Test {
    protected:
        TermController controller{};
        TermView view{controller};

        void submitAddTerm(const QString& title, const QDate& startDate, const QDate& endDate, bool active) {
            view.submitAddTerm(title, startDate, endDate, active);
        }
};


// ====================================
// FUNCTION SMOKE TESTS
// ====================================

TEST_F(TermViewTests, SubmitAddTermMutatesController) {
    QSignalSpy spy(&controller, &TermController::dataChanged);

    submitAddTerm("Fall 2025", QDate(2025, 8, 15), QDate(2025, 12, 17), true);

    ASSERT_EQ(spy.count(), 1);
    ASSERT_EQ(controller.getTermList().size(), 1);

    const Term& term = controller.findTerm("Fall 2025");
    ASSERT_EQ(term.getTitle(), "Fall 2025");
    ASSERT_TRUE(term.getActive());
}

TEST_F(TermViewTests, SubmitAddTermMultipleCallsAccumulate) {
    QSignalSpy spy(&controller, &TermController::dataChanged);

    submitAddTerm("Fall 2025", QDate(2025, 8, 15), QDate(2025, 12, 17), false);
    submitAddTerm("Spring 2026", QDate(2026, 1, 2), QDate(2026, 5, 24), true);

    ASSERT_EQ(spy.count(), 2);
    ASSERT_EQ(controller.getTermList().size(), 2);
}


// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST_F(TermViewTests, SubmitAddTermDuplicateTitleDoesNotMutate) {
    submitAddTerm("Fall 2025", QDate(2025, 8, 15), QDate(2025, 12, 17), true);

    QSignalSpy spy(&controller, &TermController::dataChanged);

    QTimer::singleShot(0, []() {
        if (QWidget* activeModal = QApplication::activeModalWidget()) {
            activeModal->close();
        }
    });

    submitAddTerm("Fall 2025", QDate(2026, 1, 2), QDate(2026, 5, 24), false);

    ASSERT_EQ(spy.count(), 0);
    ASSERT_EQ(controller.getTermList().size(), 1);
}

TEST_F(TermViewTests, SubmitAddTermEmptyTitleDoesNotMutate) {
    QSignalSpy spy(&controller, &TermController::dataChanged);

    QTimer::singleShot(0, []() {
        if (QWidget* activeModal = QApplication::activeModalWidget()) {
            activeModal->close();
        }
    });

    submitAddTerm("", QDate(2025, 8, 15), QDate(2025, 12, 17), true);

    ASSERT_EQ(spy.count(), 0);
    ASSERT_EQ(controller.getTermList().size(), 0);
}