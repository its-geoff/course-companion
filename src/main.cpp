#include <iostream>     // for I/O streams
#include <chrono>       // for date and time variables
#include "models/Assignment.hpp"
#include "models/Course.hpp"
#include "models/Term.hpp"
#include "controller/TermController.hpp"
#include "controller/MockController.hpp"
#include "view/CliView.hpp"

int main() {
    // TermController controller;
    MockController mController;
    CliView view(mController);

    view.run();

    return 0;
}