#ifndef MOCKCONTROLLER_HPP
#define MOCKCONTROLLER_HPP

/**
 * @file MockController.hpp
 * @brief Mock controller used for testing purposes. Does not include full functionality of Controller.
 * 
 * Overrides normal controller functions to allow a simplified bypass for testing.
 */

#include "view/CliView.hpp"   // for interaction with CliView

/**
 * @class MockController
 * @brief Controller with limited functionality used for testing purposes; derived from Controller.
 * 
 * Overrides normal controller functions to allow a simplified bypass for testing.
 */
class MockController : public TermController {
public:
    void addCourse(const std::string &courseName, int credits) { true; };
    void addAssignment(const std::string &courseId, const std::string &assignmentName, float grade) { true; };

    float getTermGpa() const { return 4.0; };
};

#endif  // MOCKCONTROLLER_HPP