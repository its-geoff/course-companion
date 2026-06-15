#include "view/qt/AssignmentView.hpp"

/**
 * @file AssignmentView.cpp
 * @brief Implementation of the AssignmentView class, which serves as a tertiary page for the Qt GUI.
 *
 * Displays the assignment list for a selected course. Supports add/remove operations,
 * filtering by completion status, and grade input in both percentage (95.0) and
 * point-based (47/50) formats.
 *
 * Note: the current Qt implementation uses placeholder data; controller wiring is planned but not yet implemented.
 */

#include "view/qt/FormDialog.hpp"

#include <QDate>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>

static const QString kBtnActive =
    "QPushButton {"
    "  font-size: 11px;"
    "  color: #378ADD;"
    "  background: #eef4fb;"
    "  border: 1px solid #378ADD;"
    "  border-radius: 4px;"
    "  padding: 3px 10px;"
    "}"
    "QPushButton:hover { background: #ddeaf8; }";

static const QString kBtnInactive =
    "QPushButton {"
    "  font-size: 11px;"
    "  color: #888;"
    "  background: transparent;"
    "  border: 1px solid #ddd;"
    "  border-radius: 4px;"
    "  padding: 3px 10px;"
    "}"
    "QPushButton:hover { background: #f5f5f5; }";

AssignmentView::AssignmentView(QWidget* parent) : QWidget(parent) {
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setContentsMargins(24, 24, 24, 24);
    mainLayout_->setSpacing(20);

    setupHeader();
    setupProgress();
    setupFilterBar();
    setupAssignmentList();
    mainLayout_->addStretch();
    setupFooter();
}

void AssignmentView::setupHeader() {
    auto* header       = new QWidget(this);
    auto* headerLayout = new QVBoxLayout(header);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(4);

    auto* titleRow    = new QWidget(header);
    auto* titleLayout = new QHBoxLayout(titleRow);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(8);

    courseTitle_ = new QLabel("Data Structures", titleRow);
    courseTitle_->setStyleSheet("font-size: 22px; font-weight: 500;");

    auto* typeLabel = new QLabel("Assignments", titleRow);
    typeLabel->setStyleSheet("font-size: 12px; color: #888; padding-top: 6px;");
    typeLabel->setAlignment(Qt::AlignBottom);

    addAssignmentButton_ = new QPushButton("+ Add", titleRow);
    addAssignmentButton_->setStyleSheet(
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
    connect(addAssignmentButton_, &QPushButton::clicked, this, &AssignmentView::onAddAssignment);

    removeAssignmentButton_ = new QPushButton("Remove", titleRow);
    removeAssignmentButton_->setStyleSheet(
        "QPushButton {"
        "  font-size: 12px;"
        "  color: #cc4444;"
        "  background: transparent;"
        "  border: 1px solid #cc4444;"
        "  border-radius: 4px;"
        "  padding: 3px 10px;"
        "}"
        "QPushButton:hover { background: #fdf0f0; }"
    );
    connect(removeAssignmentButton_, &QPushButton::clicked, this, &AssignmentView::onRemoveAssignment);

    titleLayout->addWidget(courseTitle_);
    titleLayout->addWidget(typeLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(addAssignmentButton_);
    titleLayout->addWidget(removeAssignmentButton_);

    dateRangeLabel_ = new QLabel("Aug 26 - Dec 20, 2024", header);
    dateRangeLabel_->setStyleSheet("font-size: 13px; color: #666;");

    headerLayout->addWidget(titleRow);
    headerLayout->addWidget(dateRangeLabel_);

    mainLayout_->addWidget(header);
}

void AssignmentView::setupProgress() {
    auto* section       = new QWidget(this);
    auto* sectionLayout = new QVBoxLayout(section);
    sectionLayout->setContentsMargins(0, 0, 0, 0);
    sectionLayout->setSpacing(6);

    auto* labelRow    = new QWidget(section);
    auto* labelLayout = new QHBoxLayout(labelRow);
    labelLayout->setContentsMargins(0, 0, 0, 0);

    auto* sectionTitle = new QLabel("ASSIGNMENT PROGRESS", labelRow);
    sectionTitle->setStyleSheet("font-size: 11px; font-weight: 500; color: #999;");

    progressLabel_ = new QLabel("9 of 14 completed", labelRow);
    progressLabel_->setStyleSheet("font-size: 12px; color: #666;");

    labelLayout->addWidget(sectionTitle);
    labelLayout->addStretch();
    labelLayout->addWidget(progressLabel_);

    progressBar_ = new QProgressBar(section);
    progressBar_->setMinimum(0);
    progressBar_->setMaximum(14);
    progressBar_->setValue(9);
    progressBar_->setTextVisible(false);
    progressBar_->setFixedHeight(6);
    progressBar_->setStyleSheet(
        "QProgressBar {"
        "  border: none;"
        "  background: #eee;"
        "  border-radius: 3px;"
        "}"
        "QProgressBar::chunk {"
        "  background: #378ADD;"
        "  border-radius: 3px;"
        "}"
    );

    sectionLayout->addWidget(labelRow);
    sectionLayout->addWidget(progressBar_);

    mainLayout_->addWidget(section);
}

void AssignmentView::setupFilterBar() {
    auto* bar    = new QWidget(this);
    auto* layout = new QHBoxLayout(bar);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);

    filterAllBtn_        = new QPushButton("All", bar);
    filterCompletedBtn_  = new QPushButton("Completed", bar);
    filterIncompleteBtn_ = new QPushButton("Incomplete", bar);

    filterAllBtn_->setStyleSheet(kBtnActive);
    filterCompletedBtn_->setStyleSheet(kBtnInactive);
    filterIncompleteBtn_->setStyleSheet(kBtnInactive);

    connect(filterAllBtn_,        &QPushButton::clicked, this, &AssignmentView::onFilterAll);
    connect(filterCompletedBtn_,  &QPushButton::clicked, this, &AssignmentView::onFilterCompleted);
    connect(filterIncompleteBtn_, &QPushButton::clicked, this, &AssignmentView::onFilterIncomplete);

    layout->addWidget(filterAllBtn_);
    layout->addWidget(filterCompletedBtn_);
    layout->addWidget(filterIncompleteBtn_);
    layout->addStretch();

    mainLayout_->addWidget(bar);
}

void AssignmentView::setupAssignmentList() {
    auto* section       = new QWidget(this);
    auto* sectionLayout = new QVBoxLayout(section);
    sectionLayout->setContentsMargins(0, 0, 0, 0);
    sectionLayout->setSpacing(8);

    auto* sectionTitle = new QLabel("ASSIGNMENTS", section);
    sectionTitle->setStyleSheet("font-size: 11px; font-weight: 500; color: #999;");
    sectionLayout->addWidget(sectionTitle);

    auto* scrollContent   = new QWidget();
    assignmentListLayout_ = new QVBoxLayout(scrollContent);
    assignmentListLayout_->setContentsMargins(0, 0, 0, 0);
    assignmentListLayout_->setSpacing(1);

    addAssignmentRow("Homework 1", "Homework · due Sep 5",   "95.0%", "A",   "4.0", true);
    addAssignmentRow("Homework 2", "Homework · due Sep 12",  "88.5%", "B+",  "3.3", true);
    addAssignmentRow("Midterm",    "Midterm · due Oct 10",   "91.0%", "A-",  "3.7", true);
    addAssignmentRow("Homework 3", "Homework · due Oct 24",  "",      "",    "",    false);
    addAssignmentRow("Final Exam", "Final Exam · due Dec 15","",      "",    "",    false);

    assignmentListLayout_->addStretch();

    auto* scrollArea = new QScrollArea(section);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setMaximumHeight(300);

    sectionLayout->addWidget(scrollArea);
    mainLayout_->addWidget(section);
}

void AssignmentView::addAssignmentRow(const QString& name, const QString& sub,
                                      const QString& pct, const QString& letter,
                                      const QString& gpa, bool completed) {
    auto* row       = new QFrame();
    auto* rowLayout = new QHBoxLayout(row);
    rowLayout->setContentsMargins(14, 10, 14, 10);

    row->setStyleSheet(
        "QFrame { background: white; border: 0.5px solid #e0e0e0; border-radius: 8px; }"
    );

    auto* dot = new QWidget(row);
    dot->setFixedSize(8, 8);
    dot->setStyleSheet(completed
        ? "background: #378ADD; border-radius: 4px;"
        : "background: #ccc; border-radius: 4px;"
    );

    auto* textCol       = new QWidget(row);
    auto* textColLayout = new QVBoxLayout(textCol);
    textColLayout->setContentsMargins(0, 0, 0, 0);
    textColLayout->setSpacing(2);

    auto* nameLabel = new QLabel(name, textCol);
    nameLabel->setStyleSheet("font-size: 13px; font-weight: 500;");

    auto* subLabel = new QLabel(sub, textCol);
    subLabel->setStyleSheet("font-size: 11px; color: #999;");

    textColLayout->addWidget(nameLabel);
    textColLayout->addWidget(subLabel);

    rowLayout->addWidget(dot, 0, Qt::AlignVCenter);
    rowLayout->addSpacing(10);
    rowLayout->addWidget(textCol, 1);

    if (completed) {
        auto* gradeCol       = new QWidget(row);
        auto* gradeColLayout = new QVBoxLayout(gradeCol);
        gradeColLayout->setContentsMargins(0, 0, 0, 0);
        gradeColLayout->setSpacing(2);
        gradeColLayout->setAlignment(Qt::AlignRight);

        auto* pctLabel = new QLabel(pct, gradeCol);
        pctLabel->setStyleSheet("font-size: 14px; font-weight: 500;");
        pctLabel->setAlignment(Qt::AlignRight);

        auto* letterLabel = new QLabel(letter, gradeCol);
        letterLabel->setStyleSheet("font-size: 11px; color: #999;");
        letterLabel->setAlignment(Qt::AlignRight);

        gradeColLayout->addWidget(pctLabel);
        gradeColLayout->addWidget(letterLabel);

        auto* gpaCol       = new QWidget(row);
        auto* gpaColLayout = new QVBoxLayout(gpaCol);
        gpaColLayout->setContentsMargins(0, 0, 0, 0);
        gpaColLayout->setSpacing(2);

        auto* gpaVal = new QLabel(gpa, gpaCol);
        gpaVal->setStyleSheet("font-size: 14px; font-weight: 500;");
        gpaVal->setAlignment(Qt::AlignRight);

        auto* gpaLbl = new QLabel("GPA pts", gpaCol);
        gpaLbl->setStyleSheet("font-size: 11px; color: #999;");
        gpaLbl->setAlignment(Qt::AlignRight);

        gpaColLayout->addWidget(gpaVal);
        gpaColLayout->addWidget(gpaLbl);

        rowLayout->addWidget(gradeCol);
        rowLayout->addSpacing(16);
        rowLayout->addWidget(gpaCol);
    } else {
        auto* pendingLabel = new QLabel("Pending", row);
        pendingLabel->setStyleSheet("font-size: 11px; color: #bbb;");
        pendingLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        rowLayout->addWidget(pendingLabel);
    }

    assignmentListLayout_->addWidget(row);
}

void AssignmentView::setupFooter() {
    auto* footer       = new QFrame(this);
    auto* footerLayout = new QHBoxLayout(footer);
    footerLayout->setContentsMargins(0, 16, 0, 0);
    footer->setStyleSheet("QFrame { border-top: 1px solid #eee; }");

    auto* avgSection = new QWidget(footer);
    auto* avgLayout  = new QVBoxLayout(avgSection);
    avgLayout->setContentsMargins(0, 0, 0, 0);
    avgLayout->setSpacing(2);

    auto* avgLbl = new QLabel("Avg grade", avgSection);
    avgLbl->setStyleSheet("font-size: 11px; color: #999;");

    avgGradeLabel_ = new QLabel("91.5%", avgSection);
    avgGradeLabel_->setStyleSheet("font-size: 16px; font-weight: 500;");

    avgLayout->addWidget(avgLbl);
    avgLayout->addWidget(avgGradeLabel_);

    auto* gpaSection = new QWidget(footer);
    auto* gpaLayout  = new QVBoxLayout(gpaSection);
    gpaLayout->setContentsMargins(0, 0, 0, 0);
    gpaLayout->setSpacing(2);

    auto* gpaLbl = new QLabel("Course GPA", gpaSection);
    gpaLbl->setStyleSheet("font-size: 11px; color: #999;");

    gpaLabel_ = new QLabel("3.74", gpaSection);
    gpaLabel_->setStyleSheet("font-size: 16px; font-weight: 500;");

    gpaLayout->addWidget(gpaLbl);
    gpaLayout->addWidget(gpaLabel_);

    footerLayout->addWidget(avgSection);
    footerLayout->addStretch();
    footerLayout->addWidget(gpaSection);

    mainLayout_->addWidget(footer);
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

void AssignmentView::onAddAssignment() {
    std::vector<FieldDef> fields = {
        { "title",     "Title",       FieldDef::Type::Text,         QString{}            },
        { "desc",      "Description", FieldDef::Type::OptionalText, QString{}            },
        { "category",  "Category",    FieldDef::Type::Text,         QString{}            },
        { "dueDate",   "Due Date",    FieldDef::Type::Date,         QDate::currentDate() },
        { "completed", "Completed",   FieldDef::Type::Bool,         false                },
    };

    FormDialog dlg("Add Assignment", fields, this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    float grade = 0.0f;
    if (dlg.boolValue("completed")) {
        bool ok = false;
        QString gradeInput = QInputDialog::getText(
            this, "Grade", "Enter grade (e.g. 95.0 or 47/50):", QLineEdit::Normal, "", &ok
        );

        if (ok && !gradeInput.trimmed().isEmpty()) {
            bool parseOk = false;
            grade = parseGradeInput(gradeInput, parseOk);

            if (!parseOk || grade < 0.0f || grade > 150.0f) {
                QMessageBox::warning(this, "Invalid Grade",
                    "Grade must be a number from 0 to 150, or a fraction like 47/50.");
                return;
            }
        }
    }

    // TODO: wire to AssignmentController::addAssignment once controller is connected
    qDebug() << "Add Assignment:"
             << dlg.textValue("title")
             << dlg.textValue("desc")
             << dlg.textValue("category")
             << dlg.dateValue("dueDate").toString("yyyy-MM-dd")
             << dlg.boolValue("completed")
             << grade;
}

void AssignmentView::onRemoveAssignment() {
    bool ok = false;
    QString title = QInputDialog::getText(
        this, "Remove Assignment", "Assignment title:", QLineEdit::Normal, "", &ok
    );

    if (!ok || title.trimmed().isEmpty())
        return;

    QMessageBox::StandardButton confirm = QMessageBox::question(
        this, "Confirm Removal",
        "Remove assignment '" + title + "'?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (confirm != QMessageBox::Yes)
        return;

    // TODO: wire to AssignmentController::removeAssignment once controller is connected
    qDebug() << "Remove Assignment:" << title;
}

void AssignmentView::onFilterAll() {
    filterAllBtn_->setStyleSheet(kBtnActive);
    filterCompletedBtn_->setStyleSheet(kBtnInactive);
    filterIncompleteBtn_->setStyleSheet(kBtnInactive);

    // TODO: show all rows once controller is connected
    qDebug() << "Filter: All";
}

void AssignmentView::onFilterCompleted() {
    filterAllBtn_->setStyleSheet(kBtnInactive);
    filterCompletedBtn_->setStyleSheet(kBtnActive);
    filterIncompleteBtn_->setStyleSheet(kBtnInactive);

    // TODO: hide incomplete rows once controller is connected
    qDebug() << "Filter: Completed";
}

void AssignmentView::onFilterIncomplete() {
    filterAllBtn_->setStyleSheet(kBtnInactive);
    filterCompletedBtn_->setStyleSheet(kBtnInactive);
    filterIncompleteBtn_->setStyleSheet(kBtnActive);

    // TODO: hide completed rows once controller is connected
    qDebug() << "Filter: Incomplete";
}