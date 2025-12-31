#ifndef MOCKCONTROLLER_HPP
#define MOCKCONTROLLER_HPP

/**
 * @file MockController.hpp
 * @brief Mock controller used for testing purposes. Does not include full functionality of TermController.
 * 
 * Overrides normal controller functions to allow a simplified bypass for testing.
 */

#include <string>       // for string functions
#include "controller/TermController.hpp"

/**
 * @class MockController
 * @brief Mock controller with limited functionality used for testing purposes; derived from TermController.
 * 
 * Overrides normal Term controller functions to allow a simplified bypass for testing.
 */
class MockController : public TermController {
public:
    void addCourse(const std::string &courseName, int credits) {};
    void addAssignment(const std::string &courseId, const std::string &assignmentName, float grade) {};

    float getTermGpa() const { return 4.0; };
};

#endif  // MOCKCONTROLLER_HPP