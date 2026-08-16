#ifndef COURSEVIEW_HPP
#define COURSEVIEW_HPP

/**
 * @file CourseView.hpp
 * @brief Definition of the CourseView class, which serves as a secondary page for the Qt GUI.
 *
 * Displays course metadata, assignment completion progress, a filterable list of assignments,
 * and a footer summary with average grade and GPA. Clicking an assignment row emits
 * assignmentSelected so MainWindow can navigate to AssignmentView. The back button emits
 * backRequested so MainWindow can navigate to TermView.
 *
 * Provides declarations only; see CourseView.cpp for implementations.
 */

#include <chrono>
#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QScrollArea>
#include <QFrame>
#include <QString>
#include <QMetaObject>
#include "controller/CourseController.hpp"

class CourseView : public QWidget {
    Q_OBJECT

    public:
        explicit CourseView(CourseController& controller, QWidget* parent = nullptr);
        void setController(CourseController& controller);

    signals:
        void assignmentSelected(const QString& assignmentTitle);
        void backRequested();

    private:
        enum class Filter { All, Completed, Incomplete };

        std::reference_wrapper<CourseController> controller_;
        Filter currentFilter_ = Filter::All;
        QVBoxLayout* mainLayout_;

        QPushButton* backButton_;
        QLabel*      courseTitle_;
        QLabel*      dateRangeLabel_;
        QPushButton* addAssignmentButton_;
        QPushButton* editCourseButton_;
        QPushButton* removeAssignmentButton_;

        QProgressBar* progressBar_;
        QLabel*       progressLabel_;

        QPushButton* filterAllBtn_;
        QPushButton* filterCompletedBtn_;
        QPushButton* filterIncompleteBtn_;

        QVBoxLayout* assignmentListLayout_;

        QLabel* avgGradeLabel_;
        QLabel* gpaLabel_;

        QMetaObject::Connection courseSelectedConn_;
        QMetaObject::Connection courseDataChangedConn_;
        QMetaObject::Connection assignmentDataChangedConn_;

        void setupHeader();
        void setupAssignmentProgress();
        void setupFilterBar();
        void setupAssignmentList();
        void setupFooter();
        QString formatDueDate(const std::chrono::year_month_day& date) const;
        void addAssignmentRow(const QString& name, const QString& sub,
                              const QString& pct, const QString& letter,
                              const QString& gpa, bool completed);
        void clearAssignmentRows();
        AssignmentController* activeAssignmentController();
        void submitEditCourse(const QString& title, const QString& description, const QDate& startDate,
                       const QDate& endDate, int numCredits, bool active);
        void submitAddAssignment(const QString& title, const QString& description, const QString& category,
                                const std::chrono::year_month_day& dueDate, bool completed, float grade);
        void submitRemoveAssignment(const QString& title);

    private slots:
        void onEditCourse();
        void onAddAssignment();
        void onRemoveAssignment();
        void onCourseSelected();
        void refreshCourse();
        void refreshAssignmentList();
        void onFilterAll();
        void onFilterCompleted();
        void onFilterIncomplete();

    friend class CourseViewTests;
};

#endif // COURSEVIEW_HPP