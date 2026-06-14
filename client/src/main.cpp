#include <QApplication>
#include <QString>
#include "controller/TermController.hpp"
#include "view/qt/MainWindow.hpp"
#include "view/cli/CliView.hpp"

int main(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (QString(argv[i]) == "--debug") {
            TermController controller;
            CliView view(controller);
            view.run();
            return 0;
        }
    }

    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}