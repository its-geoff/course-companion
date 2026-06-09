#include "view/qt/CourseView.hpp"

/**
 * @file CourseView.cpp
 * @brief Implementation of the CourseView class, which serves as a secondary page for the Qt GUI.
 *
 * Displays information received from the CourseController: course name, date range,
 * assignment completion progress, a scrollable assignment list, and a grade summary footer.
 * 
 * Note: the current Qt implementation uses placeholder data; controller wiring is planned but not yet implemented.
 */

CourseView::CourseView(QWidget* parent) : QWidget(parent) {
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setContentsMargins(24, 24, 24, 24);
    mainLayout_->setSpacing(20);

    setupHeader();
    setupAssignmentProgress();
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

    titleLayout->addWidget(courseTitle_);
    titleLayout->addWidget(courseTypeLabel);
    titleLayout->addStretch();

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

void CourseView::setupAssignmentList() {
    auto* section       = new QWidget(this);
    auto* sectionLayout = new QVBoxLayout(section);
    sectionLayout->setContentsMargins(0, 0, 0, 0);
    sectionLayout->setSpacing(8);

    auto* sectionTitle = new QLabel("ASSIGNMENTS", section);
    sectionTitle->setStyleSheet("font-size: 11px; font-weight: 500; color: #999;");
    sectionLayout->addWidget(sectionTitle);

    auto* scrollContent    = new QWidget();
    assignmentListLayout_  = new QVBoxLayout(scrollContent);
    assignmentListLayout_->setContentsMargins(0, 0, 0, 0);
    assignmentListLayout_->setSpacing(1);

    addAssignmentRow("Homework 1",  "Homework · due Sep 5",  "95.0%", "A",  "4.0");
    addAssignmentRow("Homework 2",  "Homework · due Sep 12", "88.5%", "B+", "3.3");
    addAssignmentRow("Midterm",     "Midterm · due Oct 10",  "91.0%", "A-", "3.7");
    addAssignmentRow("Homework 3",  "Homework · due Oct 24", "78.0%", "C+", "2.3");
    addAssignmentRow("Final Exam",  "Final Exam · due Dec 15","93.4%","A",  "4.0");

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
                                  const QString& gpa) {
    auto* row       = new QFrame();
    auto* rowLayout = new QHBoxLayout(row);
    rowLayout->setContentsMargins(14, 10, 14, 10);

    row->setStyleSheet(
        "QFrame { background: white; border: 0.5px solid #e0e0e0; border-radius: 8px; }"
    );

    auto* dot = new QWidget(row);
    dot->setFixedSize(8, 8);
    dot->setStyleSheet("background: #378ADD; border-radius: 4px;");

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

    rowLayout->addWidget(dot, 0, Qt::AlignVCenter);
    rowLayout->addSpacing(10);
    rowLayout->addWidget(textCol, 1);
    rowLayout->addWidget(gradeCol);
    rowLayout->addSpacing(16);
    rowLayout->addWidget(gpaCol);

    assignmentListLayout_->addWidget(row);
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

    avgGradeLabel_ = new QLabel("91.4%", avgSection);
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