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
    auto* termBtn      = new QPushButton("Fall 2024", sidebar_);
    auto* courseBtn    = new QPushButton("Data Structures", sidebar_);

    sidebarLayout->addWidget(sidebarLabel);
    sidebarLayout->addWidget(termBtn);
    sidebarLayout->addWidget(courseBtn);
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

    connect(termBtn,   &QPushButton::clicked, this, [this]() { stack_->setCurrentIndex(0); });
    connect(courseBtn, &QPushButton::clicked, this, [this]() { stack_->setCurrentIndex(1); });

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