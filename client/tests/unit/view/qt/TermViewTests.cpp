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

        void submitEditTerm(const QString& title, const QDate& startDate, const QDate& endDate, bool active) {
            view.submitEditTerm(title, startDate, endDate, active);
        }

        void submitRemoveTerm(const QString& title) {
            view.submitRemoveTerm(title);
        }

        void refreshTerm() {
            view.refreshTerm();
        }

        QString termTitleText() {
            return view.termTitle_->text();
        }

        QString termDateRangeText() {
            return view.dateRangeLabel_->text();
        }

        // dismisses the next modal that opens, used for tests that trigger QMessageBox::warning
        void dismissNextModal() {
            QTimer::singleShot(0, []() {
                if (QWidget* activeModal = QApplication::activeModalWidget()) {
                    activeModal->close();
                }
            });
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

TEST_F(TermViewTests, RefreshTermShowsNoTermSelectedByDefault) {
    ASSERT_EQ(termTitleText(), "No term selected");
    ASSERT_EQ(termDateRangeText(), "");
}

TEST_F(TermViewTests, RefreshTermShowsActiveTermAfterSelection) {
    submitAddTerm("Fall 2025", QDate(2025, 8, 15), QDate(2025, 12, 17), true);
    controller.selectTerm("Fall 2025");

    ASSERT_EQ(termTitleText(), "Fall 2025");
    ASSERT_NE(termDateRangeText(), "");
}

TEST_F(TermViewTests, SubmitEditTermChangesDatesAndActive) {
    submitAddTerm("Fall 2025", QDate(2025, 8, 15), QDate(2025, 12, 17), true);
    controller.selectTerm("Fall 2025");

    submitEditTerm("Fall 2025", QDate(2025, 9, 1), QDate(2025, 12, 20), false);

    const Term& term = controller.findTerm("Fall 2025");
    ASSERT_EQ(term.getStartDate(), std::chrono::year_month_day(2025y/9/1));
    ASSERT_EQ(term.getEndDate(), std::chrono::year_month_day(2025y/12/20));
    ASSERT_FALSE(term.getActive());
}

// regression test for the guard in submitEditTerm; resubmitting the same title used to be
// misread as a duplicate by TermController::editTitle
TEST_F(TermViewTests, SubmitEditTermWithUnchangedTitleDoesNotThrow) {
    submitAddTerm("Fall 2025", QDate(2025, 8, 15), QDate(2025, 12, 17), true);
    controller.selectTerm("Fall 2025");

    submitEditTerm("Fall 2025", QDate(2025, 9, 1), QDate(2025, 12, 20), false);

    ASSERT_EQ(controller.getTermList().size(), 1);
    ASSERT_NO_THROW(controller.findTerm("Fall 2025"));
}

TEST_F(TermViewTests, SubmitRemoveTermMutatesController) {
    submitAddTerm("Fall 2025", QDate(2025, 8, 15), QDate(2025, 12, 17), true);

    QSignalSpy spy(&controller, &TermController::dataChanged);
    submitRemoveTerm("Fall 2025");

    ASSERT_EQ(spy.count(), 1);
    ASSERT_EQ(controller.getTermList().size(), 0);
}


// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST_F(TermViewTests, SubmitAddTermDuplicateTitleDoesNotMutate) {
    submitAddTerm("Fall 2025", QDate(2025, 8, 15), QDate(2025, 12, 17), true);

    QSignalSpy spy(&controller, &TermController::dataChanged);
    dismissNextModal();

    submitAddTerm("Fall 2025", QDate(2026, 1, 2), QDate(2026, 5, 24), false);

    ASSERT_EQ(spy.count(), 0);
    ASSERT_EQ(controller.getTermList().size(), 1);
}

TEST_F(TermViewTests, SubmitAddTermEmptyTitleDoesNotMutate) {
    QSignalSpy spy(&controller, &TermController::dataChanged);
    dismissNextModal();

    submitAddTerm("", QDate(2025, 8, 15), QDate(2025, 12, 17), true);

    ASSERT_EQ(spy.count(), 0);
    ASSERT_EQ(controller.getTermList().size(), 0);
}

TEST_F(TermViewTests, SubmitEditTermNoActiveTermDoesNotCrash) {
    dismissNextModal();

    submitEditTerm("Fall 2025", QDate(2025, 8, 15), QDate(2025, 12, 17), true);

    ASSERT_EQ(controller.getTermList().size(), 0);
}

TEST_F(TermViewTests, SubmitRemoveTermNonexistentTitleDoesNotMutate) {
    submitAddTerm("Fall 2025", QDate(2025, 8, 15), QDate(2025, 12, 17), true);

    QSignalSpy spy(&controller, &TermController::dataChanged);
    dismissNextModal();

    submitRemoveTerm("Spring 2026");

    ASSERT_EQ(spy.count(), 0);
    ASSERT_EQ(controller.getTermList().size(), 1);
}