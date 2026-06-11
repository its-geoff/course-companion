#include "view/qt/TermView.hpp"

/**
 * @file TermView.cpp
 * @brief Implementation of the TermView class, which serves as a secondary page for the Qt GUI.
 *
 * This class displays information received from the TermController. The term name is shown,
 * along with course grades, timelines, and the overall GPA from the term.
 *
 * Note: the current Qt implementation uses placeholder data; controller wiring is planned but not yet implemented.
 */

#include "view/qt/FormDialog.hpp"

#include <QDate>
#include <QDebug>
#include <QPushButton>

TermView::TermView(QWidget* parent) : QWidget(parent) {
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setContentsMargins(24, 24, 24, 24);
    mainLayout_->setSpacing(20);

    setupHeader();
    setupProgress();
    setupCourseList();
    mainLayout_->addStretch();  // push the footer to the bottom
    setupFooter();
}

void TermView::setupHeader() {
    // container widget for the header section
    auto* header       = new QWidget(this);
    auto* headerLayout = new QVBoxLayout(header);
    headerLayout->setContentsMargins(0, 0, 0, 0);
    headerLayout->setSpacing(4);

    auto* titleRow    = new QWidget(header);
    auto* titleLayout = new QHBoxLayout(titleRow);
    titleLayout->setContentsMargins(0, 0, 0, 0);
    titleLayout->setSpacing(8);

    termTitle_ = new QLabel("Fall 2024", titleRow);
    termTitle_->setStyleSheet("font-size: 22px; font-weight: 500;");

    auto* termTypeLabel = new QLabel("Term", titleRow);
    termTypeLabel->setStyleSheet("font-size: 12px; color: #888; padding-top: 6px;");
    termTypeLabel->setAlignment(Qt::AlignBottom);  // align to baseline of title

    addTermButton_ = new QPushButton("+ Add Term", titleRow);
    addTermButton_->setStyleSheet(
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
    connect(addTermButton_, &QPushButton::clicked, this, &TermView::onAddTerm);

    titleLayout->addWidget(termTitle_);
    titleLayout->addWidget(termTypeLabel);
    titleLayout->addStretch();  // push both labels to the left
    titleLayout->addWidget(addTermButton_);

    dateRangeLabel_ = new QLabel("Aug 26 - Dec 20, 2024", header);
    dateRangeLabel_->setStyleSheet("font-size: 13px; color: #666;");

    headerLayout->addWidget(titleRow);
    headerLayout->addWidget(dateRangeLabel_);

    mainLayout_->addWidget(header);
}


void TermView::setupProgress() {
    auto* section       = new QWidget(this);
    auto* sectionLayout = new QVBoxLayout(section);
    sectionLayout->setContentsMargins(0, 0, 0, 0);
    sectionLayout->setSpacing(6);

    auto* labelRow    = new QWidget(section);
    auto* labelLayout = new QHBoxLayout(labelRow);
    labelLayout->setContentsMargins(0, 0, 0, 0);

    auto* sectionTitle = new QLabel("TERM PROGRESS", labelRow);
    sectionTitle->setStyleSheet("font-size: 11px; font-weight: 500; color: #999;");

    progressLabel_ = new QLabel("Week 11 of 16", labelRow);
    progressLabel_->setStyleSheet("font-size: 12px; color: #666;");

    labelLayout->addWidget(sectionTitle);
    labelLayout->addStretch();
    labelLayout->addWidget(progressLabel_);

    progressBar_ = new QProgressBar(section);
    progressBar_->setMinimum(0);
    progressBar_->setMaximum(16);
    progressBar_->setValue(11);
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


void TermView::setupCourseList() {
    auto* section       = new QWidget(this);
    auto* sectionLayout = new QVBoxLayout(section);
    sectionLayout->setContentsMargins(0, 0, 0, 0);
    sectionLayout->setSpacing(8);

    auto* sectionTitle = new QLabel("Courses", section);
    sectionTitle->setStyleSheet("font-size: 11px; font-weight: 500; color: #999; text-transform: uppercase;");
    sectionLayout->addWidget(sectionTitle);

    // the widget that holds the course rows
    auto* scrollContent    = new QWidget();
    courseListLayout_ = new QVBoxLayout(scrollContent);
    courseListLayout_->setContentsMargins(0, 0, 0, 0);
    courseListLayout_->setSpacing(1);  // tight gap between rows

    // placeholder course rows
    addCourseRow("Data Structures",  "CS 201 · 3 credits",  "93.4%", "A",  "4.0");
    addCourseRow("Linear Algebra",   "MATH 215 · 4 credits","88.1%", "B+", "3.3");
    addCourseRow("Tech Writing",     "ENG 310 · 3 credits", "91.7%", "A-", "3.7");
    addCourseRow("Ethics in CS",     "CS 401 · 3 credits",  "83.2%", "B",  "3.0");

    courseListLayout_->addStretch();

    // wrap in a scroll area
    auto* scrollArea = new QScrollArea(section);
    scrollArea->setWidget(scrollContent);
    scrollArea->setWidgetResizable(true);   // lets content resize to fill width
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    scrollArea->setMaximumHeight(300);

    sectionLayout->addWidget(scrollArea);
    mainLayout_->addWidget(section);
}


void TermView::addCourseRow(const QString& name, const QString& sub,
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

    auto* pctLabel    = new QLabel(pct, gradeCol);
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

    auto* gpaVal   = new QLabel(gpa, gpaCol);
    gpaVal->setStyleSheet("font-size: 14px; font-weight: 500;");
    gpaVal->setAlignment(Qt::AlignRight);

    auto* gpaLbl   = new QLabel("GPA pts", gpaCol);
    gpaLbl->setStyleSheet("font-size: 11px; color: #999;");
    gpaLbl->setAlignment(Qt::AlignRight);

    gpaColLayout->addWidget(gpaVal);
    gpaColLayout->addWidget(gpaLbl);

    rowLayout->addWidget(dot, 0, Qt::AlignVCenter);
    rowLayout->addSpacing(10);
    rowLayout->addWidget(textCol, 1);  // stretch factor 1 = takes remaining space
    rowLayout->addWidget(gradeCol);
    rowLayout->addSpacing(16);
    rowLayout->addWidget(gpaCol);

    courseListLayout_->addWidget(row);
}


void TermView::setupFooter() {
    auto* footer       = new QFrame(this);
    auto* footerLayout = new QHBoxLayout(footer);
    footerLayout->setContentsMargins(0, 16, 0, 0);
    footer->setStyleSheet(
        "QFrame { border-top: 1px solid #eee; }"
    );

    auto* avgSection = new QWidget(footer);
    auto* avgLayout  = new QVBoxLayout(avgSection);
    avgLayout->setContentsMargins(0, 0, 0, 0);
    avgLayout->setSpacing(2);

    auto* avgLbl = new QLabel("Avg grade", avgSection);
    avgLbl->setStyleSheet("font-size: 11px; color: #999;");

    avgGradeLabel_ = new QLabel("89.1%", avgSection);
    avgGradeLabel_->setStyleSheet("font-size: 16px; font-weight: 500;");

    avgLayout->addWidget(avgLbl);
    avgLayout->addWidget(avgGradeLabel_);

    auto* gpaSection = new QWidget(footer);
    auto* gpaLayout  = new QVBoxLayout(gpaSection);
    gpaLayout->setContentsMargins(0, 0, 0, 0);
    gpaLayout->setSpacing(2);

    auto* gpaLbl = new QLabel("Term GPA", gpaSection);
    gpaLbl->setStyleSheet("font-size: 11px; color: #999;");

    gpaLabel_ = new QLabel("3.52", gpaSection);
    gpaLabel_->setStyleSheet("font-size: 16px; font-weight: 500;");

    gpaLayout->addWidget(gpaLbl);
    gpaLayout->addWidget(gpaLabel_);

    footerLayout->addWidget(avgSection);
    footerLayout->addStretch();
    footerLayout->addWidget(gpaSection);

    mainLayout_->addWidget(footer);
}

void TermView::onAddTerm() {
    std::vector<FieldDef> fields = {
        { "title",     "Title",        FieldDef::Type::Text, QString{}             },
        { "startDate", "Start Date",   FieldDef::Type::Date, QDate::currentDate()  },
        { "endDate",   "End Date",     FieldDef::Type::Date, QDate::currentDate().addMonths(4) },
        { "active",    "Current term", FieldDef::Type::Bool, true                  },
    };

    FormDialog dlg("Add Term", fields, this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    // TODO: wire to TermController::addTerm once controller is connected
    qDebug() << "Add Term:"
             << dlg.textValue("title")
             << dlg.dateValue("startDate").toString("yyyy-MM-dd")
             << dlg.dateValue("endDate").toString("yyyy-MM-dd")
             << dlg.boolValue("active");
}