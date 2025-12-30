#include <iostream>     // for I/O streams
#include <chrono>       // for date and time variables
#include "models/Assignment.hpp"
#include "models/Course.hpp"
#include "models/Term.hpp"
#include "controller/Controller.hpp"
#include "controller/MockController.hpp"
#include "view/CliView.hpp"

Term defaultTermFromToday() {
    auto today_sys = std::chrono::system_clock::now();
    auto today_days = std::chrono::floor<std::chrono::days>(today_sys);
    std::chrono::year_month_day today{today_days};

    std::chrono::year_month_day defaultEnd = today + std::chrono::months(4);
    return Term{"Default Term", today, defaultEnd};
}

int main() {
    Term model = defaultTermFromToday();
    Controller controller(model);
    MockController mockController(controller);    // uncomment for test builds
    CliView view(mockController);       // change to controller for main builds

    view.run();

    return 0;
}