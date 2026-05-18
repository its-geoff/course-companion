#ifndef TERMVIEW_HPP
#define TERMVIEW_HPP

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