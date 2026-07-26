#ifndef PALETTEWATCHER_HPP
#define PALETTEWATCHER_HPP

/**
 * @file PaletteWatcher.hpp
 * @brief Definition of the PaletteWatcher class, which listens for system palette changes.
 *
 * Rebuilds the stylesheet whenever the OS switches between light and dark mode, keeping 
 * GUI contrast correct on any palette.
 */

#include <QEvent>
#include <QObject>

class PaletteWatcher : public QObject {
    public:
        using QObject::QObject;

    protected:
        bool eventFilter(QObject *watched, QEvent *event) override {
            if (event->type() == QEvent::ApplicationPaletteChange) {
                qApp->setStyleSheet(StyleManager::buildStylesheet());
            }
            return QObject::eventFilter(watched, event);
        }
};

#endif // PALETTEWATCHER_HPP