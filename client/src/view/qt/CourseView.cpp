#include "view/qt/CourseView.hpp"

/**
 * @file CourseView.cpp
 * @brief Implementation of the CourseView class, which serves as a secondary page for the Qt GUI.
 *
 * Displays course metadata, a filterable assignment list with add/remove support,
 * and a grade summary footer. Clicking an assignment row emits assignmentSelected.
 *
 * Note: the current Qt implementation uses placeholder data; controller wiring is planned but not yet implemented.
 */

#include "view/qt/FormDialog.hpp"

#include <QDate>
#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedLayout>

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

CourseView::CourseView(QWidget* parent) : QWidget(parent) {
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setContentsMargins(24, 24, 24, 24);
    mainLayout_->setSpacing(20);

    setupHeader();
    setupAssignmentProgress();
    setupFilterBar();
    setupAssignmentList();
    mainLayout_->addStretch();
    setupFooter();
}

void CourseView::setupHeader() {
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

    auto* courseTypeLabel = new QLabel("Course", titleRow);
    courseTypeLabel->setStyleSheet("font-size: 12px; color: #888; padding-top: 6px;");
    courseTypeLabel->setAlignment(Qt::AlignBottom);

    addCourseButton_ = new QPushButton("+ Add Course", titleRow);
    addCourseButton_->setStyleSheet(
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
    connect(addCourseButton_, &QPushButton::clicked, this, &CourseView::onAddCourse);

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
    connect(addAssignmentButton_, &QPushButton::clicked, this, &CourseView::onAddAssignment);

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
    connect(removeAssignmentButton_, &QPushButton::clicked, this, &CourseView::onRemoveAssignment);

    titleLayout->addWidget(courseTitle_);
    titleLayout->addWidget(courseTypeLabel);
    titleLayout->addStretch();
    titleLayout->addWidget(addCourseButton_);
    titleLayout->addWidget(addAssignmentButton_);
    titleLayout->addWidget(removeAssignmentButton_);

    dateRangeLabel_ = new QLabel("Aug 26 - Dec 20, 2024", header);
    dateRangeLabel_->setStyleSheet("font-size: 13px; color: #666;");

    headerLayout->addWidget(titleRow);
    headerLayout->addWidget(dateRangeLabel_);

    mainLayout_->addWidget(header);
}

void CourseView::setupAssignmentProgress() {
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

void CourseView::setupFilterBar() {
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

    connect(filterAllBtn_,        &QPushButton::clicked, this, &CourseView::onFilterAll);
    connect(filterCompletedBtn_,  &QPushButton::clicked, this, &CourseView::onFilterCompleted);
    connect(filterIncompleteBtn_, &QPushButton::clicked, this, &CourseView::onFilterIncomplete);

    layout->addWidget(filterAllBtn_);
    layout->addWidget(filterCompletedBtn_);
    layout->addWidget(filterIncompleteBtn_);
    layout->addStretch();

    mainLayout_->addWidget(bar);
}

void CourseView::setupAssignmentList() {
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

    addAssignmentRow("Homework 1", "Homework · due Sep 5",    "95.0%", "A",  "4.0", true);
    addAssignmentRow("Homework 2", "Homework · due Sep 12",   "88.5%", "B+", "3.3", true);
    addAssignmentRow("Midterm",    "Midterm · due Oct 10",    "91.0%", "A-", "3.7", true);
    addAssignmentRow("Homework 3", "Homework · due Oct 24",   "",      "",   "",    false);
    addAssignmentRow("Final Exam", "Final Exam · due Dec 15", "",      "",   "",    false);

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

void CourseView::addAssignmentRow(const QString& name, const QString& sub,
                                  const QString& pct, const QString& letter,
                                  const QString& gpa, bool completed) {
    // outer card: QStackedLayout lets the click overlay and content share the same rect
    auto* card        = new QFrame();
    auto* stackLayout = new QStackedLayout(card);
    stackLayout->setStackingMode(QStackedLayout::StackAll);

    card->setStyleSheet(
        "QFrame { background: white; border: 0.5px solid #e0e0e0; border-radius: 8px; }"
    );

    // content layer
    auto* content       = new QWidget(card);
    auto* contentLayout = new QHBoxLayout(content);
    contentLayout->setContentsMargins(14, 10, 14, 10);

    auto* dot = new QWidget(content);
    dot->setFixedSize(8, 8);
    dot->setStyleSheet(completed
        ? "background: #378ADD; border-radius: 4px;"
        : "background: #ccc; border-radius: 4px;"
    );

    auto* textCol       = new QWidget(content);
    auto* textColLayout = new QVBoxLayout(textCol);
    textColLayout->setContentsMargins(0, 0, 0, 0);
    textColLayout->setSpacing(2);

    auto* nameLabel = new QLabel(name, textCol);
    nameLabel->setStyleSheet("font-size: 13px; font-weight: 500;");

    auto* subLabel = new QLabel(sub, textCol);
    subLabel->setStyleSheet("font-size: 11px; color: #999;");

    textColLayout->addWidget(nameLabel);
    textColLayout->addWidget(subLabel);

    contentLayout->addWidget(dot, 0, Qt::AlignVCenter);
    contentLayout->addSpacing(10);
    contentLayout->addWidget(textCol, 1);

    if (completed) {
        auto* gradeCol       = new QWidget(content);
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

        auto* gpaCol       = new QWidget(content);
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

        contentLayout->addWidget(gradeCol);
        contentLayout->addSpacing(16);
        contentLayout->addWidget(gpaCol);
    } else {
        auto* pendingLabel = new QLabel("Pending", content);
        pendingLabel->setStyleSheet("font-size: 11px; color: #bbb;");
        pendingLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        contentLayout->addWidget(pendingLabel);
    }

    // click overlay: transparent button on top of content, fills the card
    auto* clickOverlay = new QPushButton(card);
    clickOverlay->setFlat(true);
    clickOverlay->setCursor(Qt::PointingHandCursor);
    clickOverlay->setStyleSheet(
        "QPushButton { background: transparent; border: none; border-radius: 8px; }"
        "QPushButton:hover { background: rgba(55, 138, 221, 0.06); }"
    );
    connect(clickOverlay, &QPushButton::clicked, this, [this, name]() {
        emit assignmentSelected(name);
    });

    stackLayout->addWidget(content);
    stackLayout->addWidget(clickOverlay);
    stackLayout->setCurrentIndex(1);

    assignmentListLayout_->addWidget(card);
}

void CourseView::setupFooter() {
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

void CourseView::onAddCourse() {
    std::vector<FieldDef> fields = {
        { "title",       "Title",          FieldDef::Type::Text,         QString{}                        },
        { "description", "Description",    FieldDef::Type::OptionalText, QString{}                        },
        { "startDate",   "Start Date",     FieldDef::Type::Date,         QDate::currentDate()             },
        { "endDate",     "End Date",       FieldDef::Type::Date,         QDate::currentDate().addMonths(4) },
        { "numCredits",  "Credits",        FieldDef::Type::Integer,      3                                },
        { "active",      "Current course", FieldDef::Type::Bool,         true                             },
    };

    FormDialog dlg("Add Course", fields, this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    // TODO: wire to CourseController::addCourse once controller is connected
    qDebug() << "Add Course:"
             << dlg.textValue("title")
             << dlg.textValue("description")
             << dlg.dateValue("startDate").toString("yyyy-MM-dd")
             << dlg.dateValue("endDate").toString("yyyy-MM-dd")
             << dlg.intValue("numCredits")
             << dlg.boolValue("active");
}

void CourseView::onAddAssignment() {
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

    // TODO: wire to AssignmentController::addAssignment once controller is connected
    qDebug() << "Add Assignment:"
             << dlg.textValue("title")
             << dlg.textValue("desc")
             << dlg.textValue("category")
             << dlg.dateValue("dueDate").toString("yyyy-MM-dd")
             << dlg.boolValue("completed");
}

void CourseView::onRemoveAssignment() {
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

void CourseView::onFilterAll() {
    filterAllBtn_->setStyleSheet(kBtnActive);
    filterCompletedBtn_->setStyleSheet(kBtnInactive);
    filterIncompleteBtn_->setStyleSheet(kBtnInactive);

    // TODO: show all rows once controller is connected
    qDebug() << "Filter: All";
}

void CourseView::onFilterCompleted() {
    filterAllBtn_->setStyleSheet(kBtnInactive);
    filterCompletedBtn_->setStyleSheet(kBtnActive);
    filterIncompleteBtn_->setStyleSheet(kBtnInactive);

    // TODO: hide incomplete rows once controller is connected
    qDebug() << "Filter: Completed";
}

void CourseView::onFilterIncomplete() {
    filterAllBtn_->setStyleSheet(kBtnInactive);
    filterCompletedBtn_->setStyleSheet(kBtnInactive);
    filterIncompleteBtn_->setStyleSheet(kBtnActive);

    // TODO: hide completed rows once controller is connected
    qDebug() << "Filter: Incomplete";
}