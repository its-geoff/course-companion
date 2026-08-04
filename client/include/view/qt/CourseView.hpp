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
 * Add, remove, and filter operations call into the AssignmentController reached through the
 * bound CourseController. CourseController is not yet a QObject (see CC-110/111), so this view
 * refreshes itself manually after each mutation rather than listening for a change signal.
 *
 * Note: course title, date range, and per-assignment letter grade/GPA are not yet wired up.
 * The model only tracks letter grade and GPA at the Course level, not per Assignment.
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
#include "controller/CourseController.hpp"

class CourseView : public QWidget {
    Q_OBJECT

    public:
        explicit CourseView(QWidget* parent = nullptr);
        void setController(CourseController* controller);

    signals:
        void assignmentSelected(const QString& assignmentTitle);
        void backRequested();

    private:
        enum class Filter { All, Completed, Incomplete };

        QVBoxLayout* mainLayout_;

        QPushButton* backButton_;
        QLabel*      courseTitle_;
        QLabel*      dateRangeLabel_;
        QPushButton* addAssignmentButton_;
        QPushButton* removeAssignmentButton_;

        QProgressBar* progressBar_;
        QLabel*       progressLabel_;

        QPushButton* filterAllBtn_;
        QPushButton* filterCompletedBtn_;
        QPushButton* filterIncompleteBtn_;

        QVBoxLayout* assignmentListLayout_;

        QLabel* avgGradeLabel_;
        QLabel* gpaLabel_;

        CourseController* controller_ = nullptr;
        Filter currentFilter_ = Filter::All;

        void setupHeader();
        void setupAssignmentProgress();
        void setupFilterBar();
        void setupAssignmentList();
        void addAssignmentRow(const QString& name, const QString& sub,
                              const QString& pct, const QString& letter,
                              const QString& gpa, bool completed);
        void setupFooter();

        AssignmentController* activeAssignmentController();
        void refreshAssignmentList();
        void clearAssignmentRows();
        QString formatDueDate(const std::chrono::year_month_day& date) const;

    private slots:
        void onAddAssignment();
        void onRemoveAssignment();
        void onFilterAll();
        void onFilterCompleted();
        void onFilterIncomplete();
};

#endif // COURSEVIEW_HPP