#include "view/qt/MainWindow.hpp"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setupUi();
}

void MainWindow::setupUi() {
    centralWidget_ = new QWidget(this);
    layout_        = new QVBoxLayout(centralWidget_);
    label_         = new QLabel("Hello from Course Companion", centralWidget_);
    button_        = new QPushButton("Click me", centralWidget_);

    layout_->addWidget(label_);
    layout_->addWidget(button_);

    setCentralWidget(centralWidget_);
    setWindowTitle("Course Companion");
    resize(800, 600);

    // connect signal to slot
    connect(button_, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
}

void MainWindow::onButtonClicked() {
    label_->setText("Button was clicked!");
}