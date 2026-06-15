#ifndef ASSIGNMENTVIEW_HPP
#define ASSIGNMENTVIEW_HPP

/**
 * @file AssignmentView.hpp
 * @brief Definition of the AssignmentView class, which serves as a tertiary page for the Qt GUI.
 *
 * Displays the assignment list for a selected course, supporting add/remove and filtering
 * by completion status. Grade input accepts both percentage format (95.0) and point-based
 * format (47/50).
 *
 * Provides declarations only; see AssignmentView.cpp for implementations.
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

class AssignmentView : public QWidget {
    Q_OBJECT

    public:
        explicit AssignmentView(QWidget* parent = nullptr);

    private slots:
        void onAddAssignment();
        void onRemoveAssignment();
        void onFilterAll();
        void onFilterCompleted();
        void onFilterIncomplete();

    private:
        void setupHeader();
        void setupProgress();
        void setupFilterBar();
        void setupAssignmentList();
        void addAssignmentRow(const QString& name, const QString& sub,
                              const QString& pct, const QString& letter,
                              const QString& gpa, bool completed);
        void setupFooter();

        float parseGradeInput(const QString& input, bool& ok) const;

        QVBoxLayout* mainLayout_;

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
};

#endif // ASSIGNMENTVIEW_HPP