#include "view/qt/MainWindow.hpp"
#include "view/qt/TermView.hpp"

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

    // sidebar layout
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

    // stacked views
    auto* termPage       = new TermView();
    auto* coursePage     = new QWidget();
    auto* assignmentPage = new QWidget();

    new QLabel("Course View", coursePage);
    new QLabel("Assignment View", assignmentPage);

    stack_->addWidget(termPage);
    stack_->addWidget(coursePage);
    stack_->addWidget(assignmentPage);
    stack_->setCurrentIndex(0);

    // root layout
    layout_->addWidget(sidebar_);
    layout_->addWidget(stack_);
    layout_->setSpacing(0);
    layout_->setContentsMargins(0, 0, 0, 0);

    setCentralWidget(centralWidget_);
    setWindowTitle("Course Companion");
    resize(900, 700);

    // signals
    connect(termBtn,   &QPushButton::clicked, this, [this]() { stack_->setCurrentIndex(0); });
    connect(courseBtn, &QPushButton::clicked, this, [this]() { stack_->setCurrentIndex(1); });
}