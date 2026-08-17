#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

/**
 * @file MainWindow.hpp
 * @brief Definition of the MainWindow class, which serves as the main page for the Qt GUI.
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
#include "controller/TermController.hpp"

class CourseView;

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget* parent = nullptr);

    private:
        TermController  controller_;
        CourseView*     coursePage_ = nullptr;    
        QWidget*        centralWidget_;
        QHBoxLayout*    layout_;
        QWidget*        sidebar_;
        QStackedWidget* stack_;
        QVBoxLayout*    termListLayout_;
        QString         selectedTermTitle_;

        void setupUi();
        void addTermRow(const Term& term);

    private slots:
        void refreshTermList();
        void onTermRowClicked(const QString& title);
        void updateTermPageVisibility();
};

#endif // MAINWINDOW_HPP