#ifndef PALETTEWATCHER_HPP
#define PALETTEWATCHER_HPP

/**
 * @file PaletteWatcher.hpp
 * @brief Definition of the PaletteWatcher class, which listens for system palette changes.
 *
 * The app is pinned to a fixed light palette (see StyleManager::fixedLightPalette). On macOS,
 * the OS still overwrites qApp's palette with system-derived colors when the user toggles OS
 * dark mode while the app is running, which fires ApplicationPaletteChange. This watcher
 * re-pins the fixed palette before rebuilding the stylesheet so that toggle doesn't leave the
 * app in a half-dark, half-light state. The reentrant_ guard exists because calling setPalette
 * from inside this handler fires ApplicationPaletteChange again; without the guard this recurses.
 */

#include <QApplication>
#include <QEvent>
#include <QObject>
#include "view/qt/StyleManager.hpp"

class PaletteWatcher : public QObject {
    public:
        using QObject::QObject;

    protected:
        bool eventFilter(QObject *watched, QEvent *event) override {
            if (event->type() == QEvent::ApplicationPaletteChange && !reentrant_) {
                reentrant_ = true;
                qApp->setPalette(StyleManager::fixedLightPalette());
                qApp->setStyleSheet(StyleManager::buildStylesheet());
                reentrant_ = false;
            }
            return QObject::eventFilter(watched, event);
        }

    private:
        bool reentrant_ = false;
};

#endif // PALETTEWATCHER_HPP