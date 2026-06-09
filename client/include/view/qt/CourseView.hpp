#ifndef COURSEVIEW_HPP
#define COURSEVIEW_HPP

/**
 * @file CourseView.hpp
 * @brief Definition of the CourseView class, which serves as a secondary page for the Qt GUI.
 *
 * Displays course metadata, assignment completion progress, a list of all assignments,
 * and a footer summary with average grade and GPA.
 * 
 * Note: the current Qt implementation uses placeholder data; controller wiring is planned but not yet implemented.
 *
 * Provides declarations only; see CourseView.cpp for implementations.
 */

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

    private:
        void setupHeader();
        void setupAssignmentProgress();
        void setupAssignmentList();
        void addAssignmentRow(const QString& name, const QString& sub,
                              const QString& pct, const QString& letter,
                              const QString& gpa);
        void setupFooter();

        QVBoxLayout* mainLayout_;

        QLabel* courseTitle_;
        QLabel* dateRangeLabel_;

        QProgressBar* progressBar_;
        QLabel* progressLabel_;

        QVBoxLayout* assignmentListLayout_;

        QLabel* avgGradeLabel_;
        QLabel* gpaLabel_;
};

#endif