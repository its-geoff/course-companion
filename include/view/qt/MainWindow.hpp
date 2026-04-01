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
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

/**
 * @class MainWindow
 * @brief Represents the main window for the Qt GUI.
 * 
 * Takes input from the user and prints output to the command line. Does not contain app logic. Interacts with the 
 * Controller to provide input to model functions and displays output after receiving return values.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() = default;

    private slots:
        void onButtonClicked();

    private:
        void setupUi();

        QWidget*     centralWidget_;
        QVBoxLayout* layout_;
        QLabel*      label_;
        QPushButton* button_;
};

#endif // MAINWINDOW_HPP