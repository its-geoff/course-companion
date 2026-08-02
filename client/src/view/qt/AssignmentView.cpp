#include "view/qt/AssignmentView.hpp"

/**
 * @file AssignmentView.cpp
 * @brief Implementation of the AssignmentView class, which serves as a detail overlay for the Qt GUI.
 *
 * Renders metadata for one selected assignment, handles grade input, lets the user edit
 * the assignment's details, and lets the user toggle its completion status. Grade input
 * accepts both percentage format (95.0) and point-based format (47/50); the corresponding
 * letter grade and percentage are calculated and displayed on submit.
 *
 * Note: grade-to-letter calculation uses placeholder thresholds; controller wiring
 * is planned but not yet implemented.
 */

#include <QDebug>
#include <QMessageBox>
#include <QStringList>
#include "view/qt/FormDialog.hpp"

AssignmentView::AssignmentView(QWidget* parent) : QWidget(parent) {
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setContentsMargins(24, 24, 24, 24);
    mainLayout_->setSpacing(20);

    setupHeader();
    setupMeta();
    setupGradeSection();
    mainLayout_->addStretch();
}

void AssignmentView::setupHeader() {
    auto* header       = new QWidget(this);
    auto* headerLayout = new QVBoxLayout(header);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(8);

    auto* topRow    = new QWidget(header);
    auto* topLayout = new QHBoxLayout(topRow);
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(8);

    backButton_ = new QPushButton("← Back", topRow);
    backButton_->setStyleSheet(
        "QPushButton {"
        "  font-size: 12px;"
        "  color: #666;"
        "  background: transparent;"
        "  border: none;"
        "  padding: 0;"
        "}"
        "QPushButton:hover { color: #378ADD; }"
    );
    connect(backButton_, &QPushButton::clicked, this, &AssignmentView::backRequested);

    topLayout->addWidget(backButton_);
    topLayout->addStretch();

    auto* titleRow    = new QWidget(header);
    auto* titleLayout = new QHBoxLayout(titleRow);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(12);

    titleLabel_ = new QLabel("Assignment Title", titleRow);
    titleLabel_->setStyleSheet("font-size: 22px; font-weight: 500; color: #1a1a1a;");

    statusBadge_ = new QLabel("Pending", titleRow);
    statusBadge_->setStyleSheet(
        "font-size: 11px;"
        "color: #888;"
        "background: #f0f0f0;"
        "border-radius: 4px;"
        "padding: 2px 8px;"
    );
    statusBadge_->setAlignment(Qt::AlignVCenter);

    editButton_ = new QPushButton("Edit", titleRow);
    editButton_->setStyleSheet(
        "QPushButton {"
        "  font-size: 12px;"
        "  color: #378ADD;"
        "  background: transparent;"
        "  border: 1px solid #378ADD;"
        "  border-radius: 4px;"
        "  padding: 3px 10px;"
        "}"
        "QPushButton:hover { background: #eef4fb; }"
    );
    connect(editButton_, &QPushButton::clicked, this, &AssignmentView::onEditDetails);

    toggleCompleteButton_ = new QPushButton("Mark Complete", titleRow);
    toggleCompleteButton_->setStyleSheet(
        "QPushButton {"
        "  font-size: 12px;"
        "  color: #378ADD;"
        "  background: transparent;"
        "  border: 1px solid #378ADD;"
        "  border-radius: 4px;"
        "  padding: 3px 10px;"
        "}"
        "QPushButton:hover { background: #eef4fb; }"
    );
    connect(toggleCompleteButton_, &QPushButton::clicked, this, &AssignmentView::onToggleCompleted);

    titleLayout->addWidget(titleLabel_);
    titleLayout->addWidget(statusBadge_);
    titleLayout->addStretch();
    titleLayout->addWidget(editButton_);
    titleLayout->addWidget(toggleCompleteButton_);

    headerLayout->addWidget(topRow);
    headerLayout->addWidget(titleRow);

    mainLayout_->addWidget(header);
}

void AssignmentView::setupMeta() {
    auto* section       = new QFrame(this);
    auto* sectionLayout = new QVBoxLayout(section);
    sectionLayout->setContentsMargins(16, 14, 16, 14);
    sectionLayout->setSpacing(10);

    section->setStyleSheet(
        "QFrame { background: white; border: 0.5px solid #e0e0e0; border-radius: 8px; }"
    );

    auto* sectionTitle = new QLabel("DETAILS", section);
    sectionTitle->setStyleSheet("font-size: 11px; font-weight: 500; color: #999;");

    auto* separator = new QFrame(section);
    separator->setFrameShape(QFrame::HLine);
    separator->setStyleSheet("color: #eee;");

    dueDateLabel_ = new QLabel("Due: —", section);
    dueDateLabel_->setStyleSheet("font-size: 13px; color: #555;");

    descriptionLabel_ = new QLabel("No description.", section);
    descriptionLabel_->setStyleSheet("font-size: 13px; color: #555;");
    descriptionLabel_->setWordWrap(true);

    sectionLayout->addWidget(sectionTitle);
    sectionLayout->addWidget(separator);
    sectionLayout->addWidget(dueDateLabel_);
    sectionLayout->addWidget(descriptionLabel_);

    mainLayout_->addWidget(section);
}

void AssignmentView::setupGradeSection() {
    gradeSection_ = new QFrame(this);
    auto* sectionLayout = new QVBoxLayout(gradeSection_);
    sectionLayout->setContentsMargins(16, 14, 16, 14);
    sectionLayout->setSpacing(12);

    gradeSection_->setStyleSheet(
        "QFrame { background: white; border: 0.5px solid #e0e0e0; border-radius: 8px; }"
    );

    gradeSectionTitle_ = new QLabel("ENTER GRADE", gradeSection_);
    gradeSectionTitle_->setStyleSheet("font-size: 11px; font-weight: 500; color: #999;");

    auto* separator = new QFrame(gradeSection_);
    separator->setFrameShape(QFrame::HLine);
    separator->setStyleSheet("color: #eee;");

    auto* inputRow    = new QWidget(gradeSection_);
    auto* inputLayout = new QHBoxLayout(inputRow);
    inputLayout->setContentsMargins(0, 0, 0, 0);
    inputLayout->setSpacing(8);

    gradeInput_ = new QLineEdit(inputRow);
    gradeInput_->setPlaceholderText("e.g. 95.0 or 47/50");
    gradeInput_->setStyleSheet(
        "QLineEdit {"
        "  font-size: 13px;"
        "  border: 1px solid #ddd;"
        "  border-radius: 4px;"
        "  padding: 5px 8px;"
        "}"
        "QLineEdit:focus { border-color: #378ADD; }"
    );

    submitGradeButton_ = new QPushButton("Save", inputRow);
    submitGradeButton_->setStyleSheet(
        "QPushButton {"
        "  font-size: 12px;"
        "  color: white;"
        "  background: #378ADD;"
        "  border: none;"
        "  border-radius: 4px;"
        "  padding: 5px 16px;"
        "}"
        "QPushButton:hover { background: #2d6fb5; }"
    );
    connect(submitGradeButton_, &QPushButton::clicked, this, &AssignmentView::onSubmitGrade);

    inputLayout->addWidget(gradeInput_, 1);
    inputLayout->addWidget(submitGradeButton_);

    gradeResultLabel_ = new QLabel("", gradeSection_);
    gradeResultLabel_->setStyleSheet("font-size: 13px; color: #555;");
    gradeResultLabel_->hide();

    sectionLayout->addWidget(gradeSectionTitle_);
    sectionLayout->addWidget(separator);
    sectionLayout->addWidget(inputRow);
    sectionLayout->addWidget(gradeResultLabel_);

    mainLayout_->addWidget(gradeSection_);
}

void AssignmentView::loadAssignment(const QString& title, const QString& description,
                                    const QString& dueDate, bool completed, float grade) {
    completed_   = completed;
    description_ = description;
    dueDate_     = dueDate;

    titleLabel_->setText(title);
    updateStatusBadge();

    dueDateLabel_->setText("Due: " + dueDate);
    descriptionLabel_->setText(description.isEmpty() ? "No description." : description);

    gradeSectionTitle_->setText(completed ? "EDIT GRADE" : "ENTER GRADE");

    gradeInput_->clear();
    gradeResultLabel_->hide();

    if (completed && grade >= 0.0f) {
        applyGradeResult(grade);
    }
}

void AssignmentView::updateStatusBadge() {
    if (completed_) {
        statusBadge_->setText("Completed");
        statusBadge_->setStyleSheet(
            "font-size: 11px;"
            "color: #378ADD;"
            "background: #eef4fb;"
            "border-radius: 4px;"
            "padding: 2px 8px;"
        );
        toggleCompleteButton_->setText("Mark Incomplete");
    } else {
        statusBadge_->setText("Pending");
        statusBadge_->setStyleSheet(
            "font-size: 11px;"
            "color: #888;"
            "background: #f0f0f0;"
            "border-radius: 4px;"
            "padding: 2px 8px;"
        );
        toggleCompleteButton_->setText("Mark Complete");
    }
}

float AssignmentView::parseGradeInput(const QString& input, bool& ok) const {
    ok = false;

    if (input.contains('/')) {
        QStringList parts = input.split('/');
        if (parts.size() != 2)
            return 0.0f;

        bool okA = false, okB = false;
        float earned = parts[0].trimmed().toFloat(&okA);
        float total  = parts[1].trimmed().toFloat(&okB);

        if (!okA || !okB || total <= 0.0f)
            return 0.0f;

        ok = true;
        return (earned / total) * 100.0f;
    }

    float pct = input.trimmed().toFloat(&ok);
    return pct;
}

void AssignmentView::applyGradeResult(float pct) {
    // placeholder grade scale matching Course::gradeScaleDefault_
    QString letter;
    float   gpaVal;

    if      (pct >= 97.0f) { letter = "A+"; gpaVal = 4.0f; }
    else if (pct >= 93.0f) { letter = "A";  gpaVal = 4.0f; }
    else if (pct >= 90.0f) { letter = "A-"; gpaVal = 3.7f; }
    else if (pct >= 87.0f) { letter = "B+"; gpaVal = 3.3f; }
    else if (pct >= 83.0f) { letter = "B";  gpaVal = 3.0f; }
    else if (pct >= 80.0f) { letter = "B-"; gpaVal = 2.7f; }
    else if (pct >= 77.0f) { letter = "C+"; gpaVal = 2.3f; }
    else if (pct >= 73.0f) { letter = "C";  gpaVal = 2.0f; }
    else if (pct >= 70.0f) { letter = "C-"; gpaVal = 1.7f; }
    else if (pct >= 67.0f) { letter = "D+"; gpaVal = 1.3f; }
    else if (pct >= 63.0f) { letter = "D";  gpaVal = 1.0f; }
    else if (pct >= 60.0f) { letter = "D-"; gpaVal = 0.7f; }
    else                   { letter = "F";  gpaVal = 0.0f; }

    gradeResultLabel_->setText(
        QString("%1%   %2   %3 GPA pts")
            .arg(QString::number(pct, 'f', 1))
            .arg(letter)
            .arg(QString::number(gpaVal, 'f', 1))
    );
    gradeResultLabel_->show();
}

void AssignmentView::onSubmitGrade() {
    QString input = gradeInput_->text().trimmed();

    if (input.isEmpty()) {
        QMessageBox::warning(this, "No Input", "Please enter a grade before saving.");
        return;
    }

    bool ok = false;
    float pct = parseGradeInput(input, ok);

    if (!ok || pct < 0.0f || pct > 150.0f) {
        QMessageBox::warning(this, "Invalid Grade",
            "Enter a percentage (e.g. 95.0) or a fraction (e.g. 47/50).\n"
            "Grade must be between 0 and 150.");
        return;
    }

    applyGradeResult(pct);

    // TODO: wire to AssignmentController::addGrade / editGrade once controller is connected
    qDebug() << "Save grade:" << pct;
}

void AssignmentView::onToggleCompleted() {
    completed_ = !completed_;
    updateStatusBadge();
    gradeSectionTitle_->setText(completed_ ? "EDIT GRADE" : "ENTER GRADE");

    if (!completed_) {
        gradeInput_->clear();
        gradeResultLabel_->hide();
    }

    // TODO: wire to AssignmentController once it exposes a direct completion toggle
    qDebug() << "Toggle completed:" << completed_;
}

void AssignmentView::onEditDetails() {
    QDate parsedDueDate = QDate::fromString(dueDate_, "MMM d, yyyy");
    if (!parsedDueDate.isValid())
        parsedDueDate = QDate::currentDate();

    std::vector<FieldDef> fields = {
        { "title",   "Title",       FieldDef::Type::Text,         titleLabel_->text() },
        { "desc",    "Description", FieldDef::Type::OptionalText, description_         },
        { "dueDate", "Due Date",    FieldDef::Type::Date,         parsedDueDate        },
    };

    FormDialog dlg("Edit Assignment", fields, this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    QString newTitle = dlg.textValue("title");
    QString newDesc  = dlg.textValue("desc");
    QDate   newDate  = dlg.dateValue("dueDate");

    titleLabel_->setText(newTitle);
    description_ = newDesc;
    dueDate_      = newDate.toString("MMM d, yyyy");

    descriptionLabel_->setText(newDesc.isEmpty() ? "No description." : newDesc);
    dueDateLabel_->setText("Due: " + dueDate_);

    // TODO: wire to AssignmentController::editTitle / editDescription / editDueDate once connected
    qDebug() << "Edit Assignment:" << newTitle << newDesc << dueDate_;
}