#ifndef TERMVIEW_HPP
#define TERMVIEW_HPP

/**
 * @file TermView.hpp
 * @brief Definition of the TermView class, which serves as a secondary page for the Qt GUI.
 * 
 * This class shows user information from the Term perspective. It displays the current term,
 * a summary of classes, and the user's overall GPA. The class sends information to the TermController and 
 * outputs results from the TermController.
 * 
 * Note: the current Qt implementation uses placeholder data; controller wiring is planned but not yet implemented.
 * 
 * Provides declarations only; see TermView.cpp for implementations.
 */

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QScrollArea>
#include <QFrame>
#include <QString>

class TermView : public QWidget {
    Q_OBJECT

    public:
        explicit TermView(QWidget* parent = nullptr);

    private:
        void setupHeader();
        void setupProgress();
        void setupCourseList();
        void addCourseRow(const QString& name, const QString& sub,
                             const QString& pct, const QString& letter,
                             const QString& gpa);
        void setupFooter();

        QVBoxLayout* mainLayout_;

        // header
        QLabel* termTitle_;
        QLabel* dateRangeLabel_;

        // progress
        QProgressBar* progressBar_;
        QLabel* progressLabel_;

        // course list
        QVBoxLayout* courseListLayout_;

        // footer
        QLabel* avgGradeLabel_;
        QLabel* gpaLabel_;
};

#endif