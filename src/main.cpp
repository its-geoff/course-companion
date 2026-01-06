#include <iostream>     // for I/O streams
#include <chrono>       // for date and time variables
#include "model/Assignment.hpp"
#include "model/Course.hpp"
#include "model/Term.hpp"
#include "controller/TermController.hpp"
#include "view/CliView.hpp"

int main() {
    TermController controller;
    CliView view(controller);

    view.run();

    return 0;
}