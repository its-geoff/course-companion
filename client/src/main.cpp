#ifdef USE_GUI

#include <QApplication>
#include "view/qt/MainWindow.hpp"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    MainWindow window;
    window.show();

    return app.exec();
}

#else

#include <iostream>
#include "controller/TermController.hpp"
#include "view/cli/CliView.hpp"

int main() {
    TermController controller;
    CliView view(controller);

    view.run();

    return 0;
}

#endif