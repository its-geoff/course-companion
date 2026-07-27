#include <QApplication>
#include <string_view>
#include "controller/TermController.hpp"
#include "view/qt/MainWindow.hpp"
#include "view/qt/StyleManager.hpp"
#include "view/qt/PaletteWatcher.hpp"
#include "view/cli/CliView.hpp"

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (std::string_view(argv[i]) == "--debug") {
            TermController controller;
            CliView view(controller);
            view.run();
            return 0;
        }
    }

    QApplication app(argc, argv);
    PaletteWatcher paletteWatcher;
    MainWindow window;

    app.setStyleSheet(StyleManager::buildStylesheet());
    app.installEventFilter(&paletteWatcher);
    window.show();

    return app.exec();
}