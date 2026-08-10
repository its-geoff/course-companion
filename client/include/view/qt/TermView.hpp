#ifndef TERMVIEW_HPP
#define TERMVIEW_HPP

/**
 * @file TermView.hpp
 * @brief Definition of the TermView class, which serves as a secondary page for the Qt GUI.
 * 
 * This class shows user information from the Term perspective. It displays the current term,
 * a summary of classes, and the user's overall GPA. The class sends information to the TermController and 
 * outputs results from the TermController. Clicking a course row emits courseSelected so MainWindow can
 * navigate to CourseView. Adding a term is triggered from the sidebar; onAddTerm is public so
 * MainWindow can call it directly.
 *
 * The course list is driven by CourseController rather than placeholder data. Since
 * TermController re-emplaces its CourseController on every selectTerm call, the connection to
 * CourseController::dataChanged has to be re-established each time termSelected fires rather
 * than made once; see onTermCourseControllerChanged. An in-page label is shown in place of the
 * list when the active term has no courses yet.
 * 
 * Provides declarations only; see TermView.cpp for implementations.
 */

#include <QDate>
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
#include "controller/TermController.hpp"
#include "controller/CourseController.hpp"

class TermView : public QWidget {
    Q_OBJECT

    public:
        explicit TermView(TermController& controller, QWidget* parent = nullptr);

    public slots:
        void onAddTerm();

    signals:
        void courseSelected(const QString& courseTitle);

    private:
        TermController& controller_;
        QVBoxLayout* mainLayout_;

        // header
        QLabel*      termTitle_;
        QLabel*      dateRangeLabel_;
        QPushButton* addCourseButton_;
        QPushButton* editTermButton_;
        QPushButton* removeTermButton_;

        // progress
        QProgressBar* progressBar_;
        QLabel* progressLabel_;

        // course list
        QVBoxLayout* courseListLayout_;
        QLabel* noCoursesLabel_;
        QMetaObject::Connection courseDataChangedConn_;

        // footer
        QLabel* avgGradeLabel_;
        QLabel* gpaLabel_;

        void setupHeader();
        void setupProgress();
        void setupCourseList();
        void setupFooter();
        void addCourseRow(const QString& name, const QString& sub,
                             const QString& pct, const QString& letter,
                             const QString& gpa);
        void clearCourseRows();
        CourseController* activeCourseController();
        void submitAddTerm(const QString& title, const QDate& startDate, const QDate& endDate, bool active);
        void submitEditTerm(const QString& title, const QDate& startDate, const QDate& endDate, bool active);
        void submitRemoveTerm(const QString& title);
        void submitAddCourse(const QString& title, const QString& description, const QDate& startDate,
                              const QDate& endDate, int numCredits, bool active);

    private slots:
        void onAddTerm();
        void onAddCourse();
        void onEditTerm();
        void onRemoveTerm();
        void refreshTerm();
        void onTermCourseControllerChanged();
        void refreshCourseList();

    friend class TermViewTests;
};

#endif