#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

/**
 * @file MainWindow.hpp
 * @brief Definition of the MainWindow class, which serves as the main page for the Qt GUI.
 * 
 * This class collects user input and presents output to the user through a Qt GUI. It does not contain any 
 * app logic. The class sends information to the Controller and outputs results from the Controller.
 * 
 * Provides declarations only; see MainWindow.cpp for implementations.
 */

#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>

/**
 * @class MainWindow
 * @brief Represents the main window for the Qt GUI.
 * 
 * Takes input from the user and displays output within the GUI. Does not contain app logic. Interacts with the
 * Controller to provide input to model functions and presents results to the user after receiving return values.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);

    private:
        void setupUi();

        QWidget*        centralWidget_;
        QHBoxLayout*    layout_;       // was QVBoxLayout*, now QHBoxLayout*
        QWidget*        sidebar_;      // new
        QStackedWidget* stack_;        // new — replaces mainArea_
};

#endif // MAINWINDOW_HPP