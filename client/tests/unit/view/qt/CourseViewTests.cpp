#include <gtest/gtest.h>
#include <QSignalSpy>
#include <QDate>
#include <QTimer>
#include <QApplication>
#include "view/qt/CourseView.hpp"
#include "controller/CourseController.hpp"

using namespace std::chrono_literals;

// test fixture for class CourseView
class CourseViewTests : public testing::Test {
    protected:
        Term term{"Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, true};
        CourseController controller{term};
        CourseView view{};

        void bindController() {
            view.setController(&controller);
        }

        void unbindController() {
            view.setController(nullptr);
        }

        void selectCourse(const std::string& title) {
            controller.selectCourse(title);
        }

        void submitAddAssignment(const QString& title, const QString& description, const QString& category,
                                  const std::chrono::year_month_day& dueDate, bool completed, float grade) {
            view.submitAddAssignment(title, description, category, dueDate, completed, grade);
        }

        void submitRemoveAssignment(const QString& title) {
            view.submitRemoveAssignment(title);
        }

        void onFilterAll() { view.onFilterAll(); }
        void onFilterCompleted() { view.onFilterCompleted(); }
        void onFilterIncomplete() { view.onFilterIncomplete(); }

        // assignmentListLayout_ always carries a trailing stretch item, so subtract it
        int assignmentRowCount() {
            return view.assignmentListLayout_->count() - 1;
        }

        QString progressLabelText() {
            return view.progressLabel_->text();
        }

        int progressBarValue() {
            return view.progressBar_->value();
        }

        int progressBarMaximum() {
            return view.progressBar_->maximum();
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

TEST_F(CourseViewTests, SetControllerWithNoCourseSelectedShowsEmptyProgress) {
    bindController();

    ASSERT_EQ(assignmentRowCount(), 0);
    ASSERT_EQ(progressLabelText(), "0 of 0 completed");
}

TEST_F(CourseViewTests, SubmitAddAssignmentMutatesAssignmentController) {
    controller.addCourse("CS 201", "Data Structures", {}, {}, 3, true);
    bindController();
    selectCourse("CS 201");

    submitAddAssignment("HW1", "", "Homework", std::chrono::year_month_day{2025y/9/10}, false, 0.0f);

    AssignmentController& assignmentController = controller.getAssignmentController();
    ASSERT_EQ(assignmentController.getAssignmentList().size(), 1);
    ASSERT_EQ(assignmentRowCount(), 1);
}

TEST_F(CourseViewTests, SubmitAddAssignmentUpdatesProgressBarAndLabel) {
    controller.addCourse("CS 201", "Data Structures", {}, {}, 3, true);
    bindController();
    selectCourse("CS 201");

    submitAddAssignment("HW1", "", "Homework", std::chrono::year_month_day{2025y/9/10}, true, 95.0f);
    submitAddAssignment("HW2", "", "Homework", std::chrono::year_month_day{2025y/9/17}, false, 0.0f);

    ASSERT_EQ(progressLabelText(), "1 of 2 completed");
    ASSERT_EQ(progressBarValue(), 1);
    ASSERT_EQ(progressBarMaximum(), 2);
}

TEST_F(CourseViewTests, SubmitRemoveAssignmentMutatesAssignmentController) {
    controller.addCourse("CS 201", "Data Structures", {}, {}, 3, true);
    bindController();
    selectCourse("CS 201");
    submitAddAssignment("HW1", "", "Homework", std::chrono::year_month_day{2025y/9/10}, false, 0.0f);

    QSignalSpy spy(&controller.getAssignmentController(), &AssignmentController::dataChanged);
    submitRemoveAssignment("HW1");

    ASSERT_EQ(spy.count(), 1);
    ASSERT_EQ(controller.getAssignmentController().getAssignmentList().size(), 0);
    ASSERT_EQ(assignmentRowCount(), 0);
}

TEST_F(CourseViewTests, FilterCompletedShowsOnlyCompletedAssignments) {
    controller.addCourse("CS 201", "Data Structures", {}, {}, 3, true);
    bindController();
    selectCourse("CS 201");
    submitAddAssignment("HW1", "", "Homework", std::chrono::year_month_day{2025y/9/10}, true, 90.0f);
    submitAddAssignment("HW2", "", "Homework", std::chrono::year_month_day{2025y/9/17}, false, 0.0f);

    onFilterCompleted();

    ASSERT_EQ(assignmentRowCount(), 1);
}

TEST_F(CourseViewTests, FilterIncompleteShowsOnlyIncompleteAssignments) {
    controller.addCourse("CS 201", "Data Structures", {}, {}, 3, true);
    bindController();
    selectCourse("CS 201");
    submitAddAssignment("HW1", "", "Homework", std::chrono::year_month_day{2025y/9/10}, true, 90.0f);
    submitAddAssignment("HW2", "", "Homework", std::chrono::year_month_day{2025y/9/17}, false, 0.0f);

    onFilterIncomplete();

    ASSERT_EQ(assignmentRowCount(), 1);
}

TEST_F(CourseViewTests, FilterAllShowsBothCompletedAndIncomplete) {
    controller.addCourse("CS 201", "Data Structures", {}, {}, 3, true);
    bindController();
    selectCourse("CS 201");
    submitAddAssignment("HW1", "", "Homework", std::chrono::year_month_day{2025y/9/10}, true, 90.0f);
    submitAddAssignment("HW2", "", "Homework", std::chrono::year_month_day{2025y/9/17}, false, 0.0f);

    onFilterIncomplete();
    ASSERT_EQ(assignmentRowCount(), 1);

    onFilterAll();
    ASSERT_EQ(assignmentRowCount(), 2);
}

// regression test for onCourseSelected: switching the active course must rewire
// AssignmentController::dataChanged to the newly selected course's controller and fully
// refresh the row list, not leave stale rows from the previously selected course
TEST_F(CourseViewTests, SwitchingCoursesRefreshesAssignmentListForNewActiveCourse) {
    controller.addCourse("CS 201", "Data Structures", {}, {}, 3, true);
    controller.addCourse("MATH 215", "Linear Algebra", {}, {}, 4, true);
    bindController();

    selectCourse("CS 201");
    submitAddAssignment("HW1", "", "Homework", std::chrono::year_month_day{2025y/9/10}, false, 0.0f);
    ASSERT_EQ(assignmentRowCount(), 1);

    selectCourse("MATH 215");

    ASSERT_EQ(assignmentRowCount(), 0);
    ASSERT_EQ(progressLabelText(), "0 of 0 completed");
}


// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST_F(CourseViewTests, SubmitAddAssignmentWithoutSelectedCourseDoesNotCrash) {
    controller.addCourse("CS 201", "Data Structures", {}, {}, 3, true);
    bindController();
    dismissNextModal();

    submitAddAssignment("HW1", "", "Homework", std::chrono::year_month_day{2025y/9/10}, false, 0.0f);

    ASSERT_EQ(assignmentRowCount(), 0);
}

TEST_F(CourseViewTests, SubmitAddAssignmentInvalidCategoryDoesNotMutate) {
    controller.addCourse("CS 201", "Data Structures", {}, {}, 3, true);
    bindController();
    selectCourse("CS 201");
    dismissNextModal();

    submitAddAssignment("HW1", "", "Not A Real Category", std::chrono::year_month_day{2025y/9/10}, false, 0.0f);

    ASSERT_EQ(controller.getAssignmentController().getAssignmentList().size(), 0);
    ASSERT_EQ(assignmentRowCount(), 0);
}

TEST_F(CourseViewTests, SubmitAddAssignmentDuplicateTitleDoesNotMutate) {
    controller.addCourse("CS 201", "Data Structures", {}, {}, 3, true);
    bindController();
    selectCourse("CS 201");
    submitAddAssignment("HW1", "", "Homework", std::chrono::year_month_day{2025y/9/10}, false, 0.0f);

    QSignalSpy spy(&controller.getAssignmentController(), &AssignmentController::dataChanged);
    dismissNextModal();

    submitAddAssignment("HW1", "", "Midterm", std::chrono::year_month_day{2025y/10/1}, false, 0.0f);

    ASSERT_EQ(spy.count(), 0);
    ASSERT_EQ(assignmentRowCount(), 1);
}

TEST_F(CourseViewTests, SubmitRemoveAssignmentNonexistentTitleDoesNotMutate) {
    controller.addCourse("CS 201", "Data Structures", {}, {}, 3, true);
    bindController();
    selectCourse("CS 201");
    submitAddAssignment("HW1", "", "Homework", std::chrono::year_month_day{2025y/9/10}, false, 0.0f);

    QSignalSpy spy(&controller.getAssignmentController(), &AssignmentController::dataChanged);
    submitRemoveAssignment("Nonexistent");

    ASSERT_EQ(spy.count(), 0);
    ASSERT_EQ(assignmentRowCount(), 1);
}

TEST_F(CourseViewTests, UnbindControllerClearsAssignmentListAndDoesNotCrash) {
    controller.addCourse("CS 201", "Data Structures", {}, {}, 3, true);
    bindController();
    selectCourse("CS 201");
    submitAddAssignment("HW1", "", "Homework", std::chrono::year_month_day{2025y/9/10}, false, 0.0f);
    ASSERT_EQ(assignmentRowCount(), 1);

    unbindController();

    ASSERT_EQ(assignmentRowCount(), 0);
    ASSERT_EQ(progressLabelText(), "0 of 0 completed");
}

TEST_F(CourseViewTests, SubmitRemoveAssignmentWithoutSelectedCourseDoesNotCrash) {
    bindController();
    dismissNextModal();

    submitRemoveAssignment("HW1");

    ASSERT_EQ(assignmentRowCount(), 0);
}