#include <iostream>
#include <chrono>
#include <cstdlib>
#include "model/Assignment.hpp"
#include "model/Course.hpp"
#include "model/Term.hpp"
#include "controller/TermController.hpp"
#include "view/CliView.hpp"
#include "db/DatabaseConnection.hpp"

int main() {
    const char* host     = std::getenv("DB_HOST");
    const char* port_str = std::getenv("DB_PORT");
    const char* user     = std::getenv("DB_USER");
    const char* password = std::getenv("DB_PASSWORD");
    const char* schema   = std::getenv("DB_SCHEMA");

    if (!host || !port_str || !user || !password || !schema) {
        std::cerr << "Missing required database environment variables.\n";
        return 1;
    }

    DatabaseConnection db(host, static_cast<unsigned int>(std::stoi(port_str)), user, password, schema);

    TermController controller(db);
    try {
        controller.loadFromDb();
    } catch (const std::exception& e) {
        std::cerr << "Failed to load data from database: " << e.what() << "\n";
        return 1;
    }

    CliView view(controller);
    view.run();

    return 0;
}