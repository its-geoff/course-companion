#ifndef ASSIGNMENTVIEW_HPP
#define ASSIGNMENTVIEW_HPP

/**
 * @file AssignmentView.hpp
 * @brief Definition of the AssignmentView class, which serves as a detail overlay for the Qt GUI.
 *
 * Shows metadata for a single selected assignment: title, description, due date, and completion
 * status. Allows grade entry (percentage or points) for incomplete assignments and grade editing
 * for completed ones. Emits backRequested when the user navigates back to CourseView.
 *
 * Provides declarations only; see AssignmentView.cpp for implementations.
 */

#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

class AssignmentView : public QWidget {
    Q_OBJECT

    public:
        explicit AssignmentView(QWidget* parent = nullptr);

        void loadAssignment(const QString& title, const QString& description,
                            const QString& dueDate, bool completed,
                            float grade = 0.0f);

    signals:
        void backRequested();

    private slots:
        void onSubmitGrade();

    private:
        void setupHeader();
        void setupMeta();
        void setupGradeSection();

        float parseGradeInput(const QString& input, bool& ok) const;
        void  applyGradeResult(float pct);

        QVBoxLayout* mainLayout_;

        QPushButton* backButton_;
        QLabel*      titleLabel_;
        QLabel*      statusBadge_;

        QLabel* descriptionLabel_;
        QLabel* dueDateLabel_;

        QWidget*     gradeSection_;
        QLabel*      gradeSectionTitle_;
        QLineEdit*   gradeInput_;
        QPushButton* submitGradeButton_;
        QLabel*      gradeResultLabel_;

        bool completed_ = false;
};

#endif // ASSIGNMENTVIEW_HPP