#ifndef ASSIGNMENTVIEW_HPP
#define ASSIGNMENTVIEW_HPP

/**
 * @file AssignmentView.hpp
 * @brief Definition of the AssignmentView class, which serves as a detail overlay for the Qt GUI.
 *
 * Shows metadata for a single selected assignment: title, description, due date, and completion
 * status. Allows editing those details, toggling completion status, and entering or editing a
 * grade. Emits backRequested when the user navigates back to CourseView.
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

    private:
        void setupHeader();
        void setupMeta();
        void setupGradeSection();

        float parseGradeInput(const QString& input, bool& ok) const;
        void  applyGradeResult(float pct);
        void  updateStatusBadge();

        QVBoxLayout* mainLayout_;

        QPushButton* backButton_;
        QPushButton* editButton_;
        QPushButton* toggleCompleteButton_;
        QLabel*      titleLabel_;
        QLabel*      statusBadge_;

        QLabel* descriptionLabel_;
        QLabel* dueDateLabel_;

        QWidget*     gradeSection_;
        QLabel*      gradeSectionTitle_;
        QLineEdit*   gradeInput_;
        QPushButton* submitGradeButton_;
        QLabel*      gradeResultLabel_;

        QString description_;
        QString dueDate_;
        bool completed_ = false;

    private slots:
        void onSubmitGrade();
        void onEditDetails();
        void onToggleCompleted();
};

#endif // ASSIGNMENTVIEW_HPP