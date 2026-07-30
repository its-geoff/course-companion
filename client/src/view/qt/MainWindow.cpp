#include "view/qt/MainWindow.hpp"
#include "view/qt/TermView.hpp"
#include "view/qt/CourseView.hpp"
#include "view/qt/AssignmentView.hpp"

/**
 * @file MainWindow.cpp
 * @brief Implementation of the MainWindow class, which serves as the main page for the Qt GUI.
 *
 * This class presents output to the user through a Qt GUI. It does not contain any
 * app logic. The class calls instances of other windows as the main driver behind the GUI.
 */

#include <QDebug>
#include <QFrame>
#include <QStackedLayout>

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

    // term details card: clicking it returns to the term page
    auto* termCard      = new QFrame(sidebar_);
    auto* termCardStack = new QStackedLayout(termCard);
    termCardStack->setStackingMode(QStackedLayout::StackAll);
    termCard->setStyleSheet(
        "QFrame { background: white; border: 0.5px solid #e0e0e0; border-radius: 8px; }"
    );

    auto* termCardContent = new QWidget(termCard);
    auto* termCardLayout  = new QVBoxLayout(termCardContent);
    termCardLayout->setContentsMargins(10, 8, 10, 8);
    termCardLayout->setSpacing(2);

    auto* termCardTitle = new QLabel("Fall 2024", termCardContent);
    termCardTitle->setStyleSheet("font-size: 13px; font-weight: 500;");

    auto* termCardDates = new QLabel("Aug 26 - Dec 20, 2024", termCardContent);
    termCardDates->setStyleSheet("font-size: 10px; color: #999;");

    termCardLayout->addWidget(termCardTitle);
    termCardLayout->addWidget(termCardDates);

    auto* termCardOverlay = new QPushButton(termCard);
    termCardOverlay->setFlat(true);
    termCardOverlay->setCursor(Qt::PointingHandCursor);
    termCardOverlay->setStyleSheet(
        "QPushButton { background: transparent; border: none; border-radius: 8px; }"
        "QPushButton:hover { background: rgba(55, 138, 221, 0.06); }"
    );

    termCardStack->addWidget(termCardContent);
    termCardStack->addWidget(termCardOverlay);
    termCardStack->setCurrentIndex(1);

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
    sidebarLayout->addWidget(termCard);
    sidebarLayout->addWidget(addTermButton);
    sidebarLayout->addStretch();

    auto* termPage       = new TermView();
    auto* coursePage     = new CourseView();
    auto* assignmentPage = new AssignmentView();

    stack_->addWidget(termPage);       // index 0
    stack_->addWidget(coursePage);     // index 1
    stack_->addWidget(assignmentPage); // index 2
    stack_->setCurrentIndex(0);

    layout_->addWidget(sidebar_);
    layout_->addWidget(stack_);
    layout_->setSpacing(0);
    layout_->setContentsMargins(0, 0, 0, 0);

    setCentralWidget(centralWidget_);
    setWindowTitle("Course Companion");
    resize(900, 700);

    connect(termCardOverlay, &QPushButton::clicked, this, [this]() { stack_->setCurrentIndex(0); });
    connect(addTermButton, &QPushButton::clicked, termPage, &TermView::onAddTerm);

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
}