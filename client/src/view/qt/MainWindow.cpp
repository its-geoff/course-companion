#include "view/qt/MainWindow.hpp"
#include "view/qt/TermView.hpp"
#include "view/qt/CourseView.hpp"
#include "view/qt/AssignmentView.hpp"

/**
 * @file MainWindow.cpp
 * @brief Implementation of the MainWindow class, which serves as the main page for the Qt GUI.
 */

#include <QDebug>
#include <QFrame>
#include <QStackedLayout>
#include <QMessageBox>
#include <sstream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUi();
}

void MainWindow::setupUi() {
    centralWidget_ = new QWidget(this);
    layout_        = new QHBoxLayout(centralWidget_);

    sidebar_ = new QWidget(centralWidget_);
    stack_   = new QStackedWidget(centralWidget_);

    sidebar_->setFixedWidth(220);
    sidebar_->setStyleSheet("background-color: #f5f5f5;");
    stack_->setStyleSheet("background-color: #ffffff;");

    auto* sidebarLayout = new QVBoxLayout(sidebar_);
    sidebarLayout->setContentsMargins(8, 16, 8, 16);
    sidebarLayout->setSpacing(4);

    auto* sidebarLabel = new QLabel("Course Companion", sidebar_);
    sidebarLabel->setStyleSheet("font-size: 13px; font-weight: 600; color: #333;");

    auto* termListWidget = new QWidget(sidebar_);
    termListLayout_ = new QVBoxLayout(termListWidget);
    termListLayout_->setContentsMargins(0, 0, 0, 0);
    termListLayout_->setSpacing(6);

    auto* addTermButton = new QPushButton("+ Add Term", sidebar_);
    addTermButton->setStyleSheet(
        "QPushButton {"
        "  font-size: 12px;"
        "  color: #378ADD;"
        "  background: transparent;"
        "  border: 1px solid #378ADD;"
        "  border-radius: 4px;"
        "  padding: 4px 0;"
        "}"
        "QPushButton:hover { background: #eef4fb; }"
    );

    sidebarLayout->addWidget(sidebarLabel);
    sidebarLayout->addSpacing(8);
    sidebarLayout->addWidget(termListWidget);
    sidebarLayout->addStretch();
    sidebarLayout->addWidget(addTermButton);

    auto* termPage       = new TermView(controller_);
    auto* coursePage     = new CourseView();
    auto* assignmentPage = new AssignmentView();

    // shown whenever no term is currently active, instead of TermView's own placeholder state
    auto* emptyStatePage = new QWidget();
    auto* emptyLayout    = new QVBoxLayout(emptyStatePage);
    auto* emptyLabel     = new QLabel("Select a term to begin", emptyStatePage);
    emptyLabel->setAlignment(Qt::AlignCenter);
    emptyLabel->setStyleSheet("font-size: 14px; color: #999;");
    emptyLayout->addWidget(emptyLabel);

    stack_->addWidget(termPage);       // index 0
    stack_->addWidget(coursePage);     // index 1
    stack_->addWidget(assignmentPage); // index 2
    stack_->addWidget(emptyStatePage); // index 3
    stack_->setCurrentIndex(3);        // no term selected at startup

    layout_->addWidget(sidebar_);
    layout_->addWidget(stack_);
    layout_->setSpacing(0);
    layout_->setContentsMargins(0, 0, 0, 0);

    setCentralWidget(centralWidget_);
    setWindowTitle("Course Companion");
    resize(900, 700);

    connect(addTermButton, &QPushButton::clicked, termPage, &TermView::onAddTerm);
    connect(&controller_, &TermController::dataChanged, this, &MainWindow::refreshTermList);
    connect(&controller_, &TermController::dataChanged, this, &MainWindow::updateTermPageVisibility);

    connect(termPage, &TermView::courseSelected, this,
        [this](const QString& title) {
            // TODO: load real course data from controller once wiring lands
            qDebug() << "Course selected:" << title;
            stack_->setCurrentIndex(1);
        }
    );

    connect(coursePage, &CourseView::backRequested, this,
        [this]() { stack_->setCurrentIndex(0); }
    );

    connect(coursePage, &CourseView::assignmentSelected, this,
        [this, assignmentPage](const QString& title) {
            // TODO: fetch real data from controller; placeholder values used until then
            assignmentPage->loadAssignment(
                title,
                "",
                "Dec 15, 2024",
                false,
                0.0f
            );
            stack_->setCurrentIndex(2);
        }
    );

    connect(assignmentPage, &AssignmentView::backRequested, this,
        [this]() { stack_->setCurrentIndex(1); }
    );

    refreshTermList();
}

void MainWindow::refreshTermList() {
    QLayoutItem* item;
    while ((item = termListLayout_->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    for (const std::string& id : controller_.getTermOrder()) {
        addTermRow(controller_.getTermList().at(id));
    }

    termListLayout_->addStretch();
}

void MainWindow::addTermRow(const Term& term) {
    QString title = QString::fromStdString(term.getTitle());
    bool isSelected = (title == selectedTermTitle_);

    auto* card = new QFrame(sidebar_);
    card->setFixedHeight(52);
    auto* cardStack = new QStackedLayout(card);
    cardStack->setStackingMode(QStackedLayout::StackAll);
    card->setStyleSheet(isSelected
        ? "QFrame { background: #eef4fb; border: 1px solid #378ADD; border-radius: 8px; }"
        : "QFrame { background: white; border: 0.5px solid #e0e0e0; border-radius: 8px; }"
    );

    auto* content       = new QWidget(card);
    auto* contentLayout = new QVBoxLayout(content);
    contentLayout->setContentsMargins(10, 8, 10, 8);
    contentLayout->setSpacing(2);

    auto* titleLabel = new QLabel(title, content);
    titleLabel->setStyleSheet("font-size: 13px; font-weight: 500; color: #1a1a1a;");

    std::ostringstream dateStream;
    dateStream << term.getStartDate() << " - " << term.getEndDate();
    auto* datesLabel = new QLabel(QString::fromStdString(dateStream.str()), content);
    datesLabel->setStyleSheet("font-size: 10px; color: #999;");

    contentLayout->addWidget(titleLabel);
    contentLayout->addWidget(datesLabel);

    auto* overlay = new QPushButton(card);
    overlay->setFlat(true);
    overlay->setCursor(Qt::PointingHandCursor);
    overlay->setFocusPolicy(Qt::NoFocus);
    overlay->setAccessibleName(QString("Open term %1").arg(title));
    overlay->setStyleSheet(
        "QPushButton { background: transparent; border: none; border-radius: 8px; }"
        "QPushButton:hover { background: rgba(55, 138, 221, 0.06); }"
    );
    connect(overlay, &QPushButton::clicked, this, [this, title]() { onTermRowClicked(title); });

    cardStack->addWidget(content);
    cardStack->addWidget(overlay);
    cardStack->setCurrentIndex(1);

    termListLayout_->addWidget(card);
}

void MainWindow::onTermRowClicked(const QString& title) {
    try {
        controller_.selectTerm(title.toStdString());
        selectedTermTitle_ = title;
        refreshTermList();
        stack_->setCurrentIndex(0);
    } catch (const std::out_of_range& e) {
        QMessageBox::warning(this, "Select Term Failed", QString::fromStdString(e.what()));
    }
}

// clears the highlighted sidebar row and switches away from the term page whenever there is no
// active term, e.g. it was just removed; runs regardless of which page is currently showing so a
// stale highlight can't survive into a later term reusing the same title
void MainWindow::updateTermPageVisibility() {
    try {
        controller_.getActiveTerm();
    } catch (const std::logic_error& e) {
        selectedTermTitle_.clear();

        if (stack_->currentIndex() == 0) {
            stack_->setCurrentIndex(3);
        }
    }
}