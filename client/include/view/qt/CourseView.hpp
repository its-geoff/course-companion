#ifndef COURSEVIEW_HPP
#define COURSEVIEW_HPP

/**
 * @file CourseView.hpp
 * @brief Definition of the CourseView class, which serves as a secondary page for the Qt GUI.
 *
 * Displays course metadata, assignment completion progress, a filterable list of assignments,
 * and a footer summary with average grade and GPA. Clicking an assignment row emits
 * assignmentSelected so MainWindow can navigate to AssignmentView.
 *
 * Provides declarations only; see CourseView.cpp for implementations.
 */

#include <QPushButton>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QScrollArea>
#include <QFrame>
#include <QString>

class CourseView : public QWidget {
    Q_OBJECT

    public:
        explicit CourseView(QWidget* parent = nullptr);

    signals:
        void assignmentSelected(const QString& assignmentTitle);

    private slots:
        void onAddCourse();
        void onAddAssignment();
        void onRemoveAssignment();
        void onFilterAll();
        void onFilterCompleted();
        void onFilterIncomplete();

    private:
        void setupHeader();
        void setupAssignmentProgress();
        void setupFilterBar();
        void setupAssignmentList();
        void addAssignmentRow(const QString& name, const QString& sub,
                              const QString& pct, const QString& letter,
                              const QString& gpa, bool completed);
        void setupFooter();

        QVBoxLayout* mainLayout_;

        QLabel*      courseTitle_;
        QLabel*      dateRangeLabel_;
        QPushButton* addCourseButton_;
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
};

#endif // COURSEVIEW_HPP