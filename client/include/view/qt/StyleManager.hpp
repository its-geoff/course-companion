#ifndef STYLEMANAGER_HPP
#define STYLEMANAGER_HPP

/**
 * @file StyleManager.hpp
 * @brief Definition of the StyleManager namespace, which standardizes style information for Qt.
 *
 * Sets all color palette information for the Qt GUI. Imported by all pages of the GUI for consistent
 * branding and style.
 *
 * The app uses the Fusion QStyle (set in main.cpp before any palette or widget is created) rather
 * than the native per-platform style. Fusion paints every widget, including line edits and combo
 * boxes, entirely from QPalette, so a palette set here is honored everywhere with no native
 * rendering exceptions. This is what makes fixedLightPalette() an actual single source of truth
 * across platforms, and is also what will make future theme switching (light/dark/custom) work
 * uniformly if that gets added: swapping the QPalette returned to the app is enough, no per-widget
 * or per-platform special casing needed.
 *
 * fixedLightPalette() is the current palette; main.cpp applies it at startup and PaletteWatcher
 * re-applies it if the OS palette changes underneath the app while running.
 */

#include <QPalette>
#include <QColor>
#include <QString>
#include <QApplication>

namespace StyleManager {
    inline QPalette fixedLightPalette() {
        QPalette palette;
        palette.setColor(QPalette::Window, QColor("#ffffff"));
        palette.setColor(QPalette::WindowText, QColor("#1a1a1a"));
        palette.setColor(QPalette::Base, QColor("#ffffff"));
        palette.setColor(QPalette::Text, QColor("#1a1a1a"));
        palette.setColor(QPalette::Button, QColor("#f0f0f0"));
        palette.setColor(QPalette::ButtonText, QColor("#1a1a1a"));
        return palette;
    }

    inline QString buildStylesheet() {
        const QPalette palette = qApp->palette();

        const QString windowBg      = palette.color(QPalette::Window).name();
        const QString windowText    = palette.color(QPalette::WindowText).name();
        const QString base          = palette.color(QPalette::Base).name();
        const QString buttonBg      = palette.color(QPalette::Button).name();
        const QString buttonText    = palette.color(QPalette::ButtonText).name();

        // keep #378ADD for brand consistency
        return QString(R"(
            QWidget {
                background-color: %1;
                color: %2;
            }
            QListWidget, QTreeWidget, QListView, QTreeView {
                background-color: %1;
                color: %2;
            }
            QListWidget::item, QTreeWidget::item {
                color: %2;
            }
            QListWidget::item:selected, QTreeWidget::item:selected {
                background-color: %6;
                color: %7;
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
        .arg(buttonBg)
        .arg(buttonText)
        .arg(palette.color(QPalette::Highlight).name())
        .arg(palette.color(QPalette::HighlightedText).name());
    }
}

#endif // STYLEMANAGER_HPP