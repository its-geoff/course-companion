#ifndef STYLEMANAGER_HPP
#define STYLEMANAGER_HPP

/**
 * @file StyleManager.hpp
 * @brief Definition of the StyleManager class, which standardizes style information for Qt.
 *
 * Sets all color palette information for the Qt GUI. Imported by all pages of the GUI for consistent
 * branding and style.
 */

#include <QPalette>
#include <QString>
#include <QApplication>

namespace StyleManager {
    inline QString buildStylesheet() {
        const QPalette palette = qApp->palette();

        const QString windowBg      = palette.color(QPalette::Window).name();
        const QString windowText    = palette.color(QPalette::WindowText).name();
        const QString base          = palette.color(QPalette::Base).name();
        const QString altBase       = palette.color(QPalette::AlternateBase).name();
        const QString buttonBg      = palette.color(QPalette::Button).name();
        const QString buttonText    = palette.color(QPalette::ButtonText).name();

        // keep #378ADD for brand consistency
        return QString(R"(
            QWidget {
                background-color: %1;
                color: %2;
            }
            QLineEdit, QTextEdit {
                background-color: %3;
                color: %2;
            }
            QPushButton {
                background-color: %4;
                color: %5;
            }
            QPushButton:hover {
                background-color: #378ADD;
                color: white;
            }
        )").arg(windowBg)
           .arg(windowText)
           .arg(base)
           .arg(altBase)
           .arg(buttonBg)
           .arg(buttonText);
    }
}

#endif // STYLEMANAGER_HPP