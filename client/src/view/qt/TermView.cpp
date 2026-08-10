#include "view/qt/TermView.hpp"

/**
 * @file TermView.cpp
 * @brief Implementation of the TermView class, which serves as a secondary page for the Qt GUI.
 *
 * This class displays information received from the TermController. The term name is shown,
 * along with course grades, timelines, and the overall GPA from the term. Clicking a course
 * card emits courseSelected so MainWindow can navigate to CourseView.
 *
 * The course list is populated from CourseController::getCourseList and refreshes in response
 * to CourseController::dataChanged and TermController::termSelected (the latter to rewire the
 * connection whenever the active term, and therefore the active CourseController, changes).
 */

#include <QDebug>
#include <QMessageBox>
#include <QPushButton>
#include <QStackedLayout>
#include <sstream>
#include "utils/utils.hpp"
#include "view/qt/FormDialog.hpp"

TermView::TermView(TermController& controller, QWidget* parent) 
    : QWidget(parent), controller_{controller} {
    mainLayout_ = new QVBoxLayout(this);
    mainLayout_->setContentsMargins(24, 24, 24, 24);
    mainLayout_->setSpacing(20);

    setupHeader();
    setupProgress();
    setupCourseList();
    mainLayout_->addStretch();  // push the footer to the bottom
    setupFooter();

    connect(&controller_, &TermController::dataChanged, this, &TermView::refreshTerm);
    connect(&controller_, &TermController::termSelected, this, &TermView::refreshTerm);
    connect(&controller_, &TermController::termSelected, this, &TermView::onTermCourseControllerChanged);

    refreshTerm();
    onTermCourseControllerChanged();
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

    termTitle_ = new QLabel("No term selected", titleRow);
    termTitle_->setStyleSheet("font-size: 22px; font-weight: 500; color: #1a1a1a;");

    editTermButton_ = new QPushButton("Edit", titleRow);
    editTermButton_->setStyleSheet(
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
    connect(editTermButton_, &QPushButton::clicked, this, &TermView::onEditTerm);

    removeTermButton_ = new QPushButton("Remove", titleRow);
    removeTermButton_->setStyleSheet(
        "QPushButton {"
        "  font-size: 12px;"
        "  color: #d9534f;"
        "  background: transparent;"
        "  border: 1px solid #d9534f;"
        "  border-radius: 4px;"
        "  padding: 3px 10px;"
        "}"
        "QPushButton:hover { background: #fdecea; }"
    );
    connect(removeTermButton_, &QPushButton::clicked, this, &TermView::onRemoveTerm);

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
    connect(addCourseButton_, &QPushButton::clicked, this, &TermView::onAddCourse);

    titleLayout->addWidget(termTitle_);
    titleLayout->addStretch();
    titleLayout->addWidget(editTermButton_);
    titleLayout->addWidget(removeTermButton_);
    titleLayout->addWidget(addCourseButton_);

    auto* termTypeLabel = new QLabel("Term", header);
    termTypeLabel->setStyleSheet("font-size: 14px; font-weight: 500; color: #888;");

    dateRangeLabel_ = new QLabel("", header);
    dateRangeLabel_->setStyleSheet("font-size: 13px; color: #666;");

    headerLayout->addWidget(titleRow);
    headerLayout->addWidget(termTypeLabel);
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

    // shown instead of the list when the active term has no courses; lives outside
    // courseListLayout_ so clearCourseRows doesn't destroy it on every refresh
    noCoursesLabel_ = new QLabel("No courses yet. Add one to get started.", section);
    noCoursesLabel_->setStyleSheet("font-size: 12px; color: #999;");
    noCoursesLabel_->setAlignment(Qt::AlignCenter);
    noCoursesLabel_->setContentsMargins(0, 12, 0, 12);
    noCoursesLabel_->hide();
    sectionLayout->addWidget(noCoursesLabel_);

    // the widget that holds the course rows
    auto* scrollContent    = new QWidget();
    courseListLayout_ = new QVBoxLayout(scrollContent);
    courseListLayout_->setContentsMargins(0, 0, 0, 0);
    courseListLayout_->setSpacing(6);
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
    avgGradeLabel_->setStyleSheet("font-size: 16px; font-weight: 500; color: #1a1a1a;");

    avgLayout->addWidget(avgLbl);
    avgLayout->addWidget(avgGradeLabel_);

    auto* gpaSection = new QWidget(footer);
    auto* gpaLayout  = new QVBoxLayout(gpaSection);
    gpaLayout->setContentsMargins(0, 0, 0, 0);
    gpaLayout->setSpacing(2);

    auto* gpaLbl = new QLabel("Term GPA", gpaSection);
    gpaLbl->setStyleSheet("font-size: 11px; color: #999;");

    gpaLabel_ = new QLabel("3.52", gpaSection);
    gpaLabel_->setStyleSheet("font-size: 16px; font-weight: 500; color: #1a1a1a;");

    gpaLayout->addWidget(gpaLbl);
    gpaLayout->addWidget(gpaLabel_);

    footerLayout->addWidget(avgSection);
    footerLayout->addStretch();
    footerLayout->addWidget(gpaSection);

    mainLayout_->addWidget(footer);
}

void TermView::addCourseRow(const QString& name, const QString& sub,
                             const QString& pct, const QString& letter,
                             const QString& gpa) {
    // outer card: QStackedLayout lets the click overlay and content share the same rect
    auto* card        = new QFrame();
    card->setFixedHeight(52);
    auto* stackLayout = new QStackedLayout(card);
    stackLayout->setStackingMode(QStackedLayout::StackAll);

    card->setStyleSheet(
        "QFrame { background: white; border: 0.5px solid #e0e0e0; border-radius: 8px; }"
    );

    auto* content   = new QWidget(card);
    auto* rowLayout = new QHBoxLayout(content);
    rowLayout->setContentsMargins(10, 8, 10, 8);

    auto* dot = new QWidget(content);
    dot->setFixedSize(8, 8);
    dot->setStyleSheet("background: #378ADD; border-radius: 4px;");

    auto* textCol       = new QWidget(content);
    auto* textColLayout = new QVBoxLayout(textCol);
    textColLayout->setContentsMargins(0, 0, 0, 0);
    textColLayout->setSpacing(2);

    auto* nameLabel = new QLabel(name, textCol);
    nameLabel->setStyleSheet("font-size: 13px; font-weight: 500; color: #1a1a1a;");

    auto* subLabel = new QLabel(sub, textCol);
    subLabel->setStyleSheet("font-size: 11px; color: #999;");

    textColLayout->addWidget(nameLabel);
    textColLayout->addWidget(subLabel);

    auto* gradeCol       = new QWidget(content);
    auto* gradeColLayout = new QVBoxLayout(gradeCol);
    gradeColLayout->setContentsMargins(0, 0, 0, 0);
    gradeColLayout->setSpacing(2);
    gradeColLayout->setAlignment(Qt::AlignRight);

    auto* pctLabel = new QLabel(pct, gradeCol);
    pctLabel->setStyleSheet("font-size: 14px; font-weight: 500; color: #1a1a1a;");
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
    gpaVal->setStyleSheet("font-size: 14px; font-weight: 500; color: #1a1a1a;");
    gpaVal->setAlignment(Qt::AlignRight);

    auto* gpaLbl = new QLabel("GPA pts", gpaCol);
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

    // click overlay: transparent button on top of content, fills the card
    auto* clickOverlay = new QPushButton(card);
    clickOverlay->setFlat(true);
    clickOverlay->setCursor(Qt::PointingHandCursor);
    clickOverlay->setFocusPolicy(Qt::NoFocus);
    clickOverlay->setAccessibleName(QString("Open course %1").arg(name));
    clickOverlay->setStyleSheet(
        "QPushButton { background: transparent; border: none; border-radius: 8px; }"
        "QPushButton:hover { background: rgba(55, 138, 221, 0.06); }"
    );
    connect(clickOverlay, &QPushButton::clicked, this, [this, name]() {
        emit courseSelected(name);
    });

    stackLayout->addWidget(content);
    stackLayout->addWidget(clickOverlay);
    stackLayout->setCurrentIndex(1);

    // insert before the trailing stretch item
    courseListLayout_->insertWidget(courseListLayout_->count() - 1, card);
}

void TermView::clearCourseRows() {
    QLayoutItem* item;
    while ((item = courseListLayout_->takeAt(0)) != nullptr) {
        if (QWidget* widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

CourseController* TermView::activeCourseController() {
    try {
        return &controller_.getCourseController();
    } catch (const std::exception& e) {
        return nullptr;
    }
}

void TermView::submitAddTerm(const QString& title, const QDate& startDate, const QDate& endDate, bool active) {
    try {
        controller_.addTerm(
            title.toStdString(),
            utils::parseDateFromQt(startDate),
            utils::parseDateFromQt(endDate),
            active
        );
    } catch (const std::logic_error& e) {
        QMessageBox::warning(this, "Add Term Failed", QString::fromStdString(e.what()));
    }
}

// only calls editTitle if the title actually changed, since TermController::editTitle treats a resubmitted, unchanged title as a duplicate
void TermView::submitEditTerm(const QString& title, const QDate& startDate, const QDate& endDate, bool active) {
    try {
        const Term& term = controller_.getActiveTerm();
        std::string id = term.getId();

        if (title.toStdString() != term.getTitle()) {
            controller_.editTitle(id, title.toStdString());
        }
        controller_.editStartDate(id, utils::parseDateFromQt(startDate));
        controller_.editEndDate(id, utils::parseDateFromQt(endDate));
        controller_.editActive(id, active);
    } catch (const std::logic_error& e) {
        QMessageBox::warning(this, "Edit Term Failed", QString::fromStdString(e.what()));
    }
}

void TermView::submitRemoveTerm(const QString& title) {
    try {
        controller_.removeTerm(title.toStdString());
    } catch (const std::out_of_range& e) {
        QMessageBox::warning(this, "Remove Term Failed", QString::fromStdString(e.what()));
    }
}

void TermView::submitAddCourse(const QString& title, const QString& description, const QDate& startDate,
                                const QDate& endDate, int numCredits, bool active) {
    CourseController* courseController = activeCourseController();
    if (!courseController) {
        QMessageBox::warning(this, "Add Course Failed", "No term is currently selected.");
        return;
    }

    try {
        courseController->addCourse(
            title.toStdString(),
            description.toStdString(),
            utils::parseDateFromQt(startDate),
            utils::parseDateFromQt(endDate),
            numCredits,
            active
        );
    } catch (const std::logic_error& e) {
        QMessageBox::warning(this, "Add Course Failed", QString::fromStdString(e.what()));
    } catch (const std::exception& e) {
        QMessageBox::warning(this, "Add Course Failed", "An unexpected error occurred while adding the course.");
    }
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

    submitAddTerm(dlg.textValue("title"), dlg.dateValue("startDate"), dlg.dateValue("endDate"), dlg.boolValue("active"));
}

void TermView::onAddCourse() {
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

    submitAddCourse(
        dlg.textValue("title"),
        dlg.textValue("description"),
        dlg.dateValue("startDate"),
        dlg.dateValue("endDate"),
        dlg.intValue("numCredits"),
        dlg.boolValue("active")
    );
}

// pre-fills the edit dialog with the active term's current values
void TermView::onEditTerm() {
    const Term* term = nullptr;
    try {
        term = &controller_.getActiveTerm();
    } catch (const std::logic_error& e) {
        QMessageBox::warning(this, "Edit Term Failed", "No term is currently selected.");
        return;
    }

    std::vector<FieldDef> fields = {
        { "title",     "Title",        FieldDef::Type::Text, QString::fromStdString(term->getTitle()) },
        { "startDate", "Start Date",   FieldDef::Type::Date, utils::parseDateToQt(term->getStartDate()) },
        { "endDate",   "End Date",     FieldDef::Type::Date, utils::parseDateToQt(term->getEndDate()) },
        { "active",    "Current term", FieldDef::Type::Bool, term->getActive() },
    };

    FormDialog dlg("Edit Term", fields, this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    submitEditTerm(dlg.textValue("title"), dlg.dateValue("startDate"), dlg.dateValue("endDate"), dlg.boolValue("active"));
}

// confirms with the user before removing the active term, since removal cannot be undone
void TermView::onRemoveTerm() {
    QString title;
    try {
        title = QString::fromStdString(controller_.getActiveTerm().getTitle());
    } catch (const std::logic_error& e) {
        QMessageBox::warning(this, "Remove Term Failed", "No term is currently selected.");
        return;
    }

    auto result = QMessageBox::question(this, "Remove Term",
        QString("Remove term \"%1\"? This cannot be undone.").arg(title));

    if (result != QMessageBox::Yes)
        return;

    submitRemoveTerm(title);
}

// pulls the currently active term from the controller and updates the header; falls back to a placeholder if nothing is selected
void TermView::refreshTerm() {
    try {
        const Term& term = controller_.getActiveTerm();
        termTitle_->setText(QString::fromStdString(term.getTitle()));

        std::ostringstream dateStream;
        dateStream << term.getStartDate() << " - " << term.getEndDate();
        dateRangeLabel_->setText(QString::fromStdString(dateStream.str()));
    } catch (const std::logic_error& e) {
        termTitle_->setText("No term selected");
        dateRangeLabel_->setText("");
    }
}

// reconnects the CourseController::dataChanged signal to whichever CourseController is
// currently active, since TermController re-emplaces it on every selectTerm call and a fresh
// QObject means a fresh set of connections
void TermView::onTermCourseControllerChanged() {
    disconnect(courseDataChangedConn_);

    CourseController* courseController = activeCourseController();
    if (courseController) {
        courseDataChangedConn_ = connect(courseController, &CourseController::dataChanged,
                                          this, &TermView::refreshCourseList);
    }

    refreshCourseList();
}

void TermView::refreshCourseList() {
    clearCourseRows();
    courseListLayout_->addStretch();

    CourseController* courseController = activeCourseController();
    if (!courseController) {
        noCoursesLabel_->show();
        return;
    }

    const auto& courseList = courseController->getCourseList();
    const auto& courseOrder = courseController->getCourseOrder();
    noCoursesLabel_->setVisible(courseOrder.empty());

    for (const std::string& id : courseOrder) {
        const Course& course = courseList.at(id);

        QString name = QString::fromStdString(course.getTitle());

        QString description = QString::fromStdString(course.getDescription());
        QString credits = QString("%1 credit%2").arg(course.getNumCredits()).arg(course.getNumCredits() == 1 ? "" : "s");
        QString sub = description.isEmpty() ? credits : description + " · " + credits;

        QString pct = QString::number(course.getGradePct(), 'f', 1) + "%";
        QString letter = QString::fromStdString(course.getLetterGrade());
        QString gpa = QString::number(course.getGpaVal(), 'f', 1);

        addCourseRow(name, sub, pct, letter, gpa);
    }
}