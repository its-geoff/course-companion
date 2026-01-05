#include <gtest/gtest.h>
#include <sstream>      // diverts output from terminal to separate stream
#include <regex>        // regular expression matching for UUIDs
#include "controller/TermController.hpp"
#include "utils/utils.hpp"

using namespace std::chrono_literals;

// test fixture for class TermController
class TermControllerTest : public testing::Test {
    protected:
        TermController controller{};
};

// ====================================
// GETTER SMOKE TESTS
// ====================================

TEST_F(TermControllerTest, TermGetter) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    controller.addTerm("Spring 2026", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/24}, true);

    const Term& selectedTerm = controller.getTerm("Spring 2026");
    ASSERT_EQ(selectedTerm.getTitle(), "Spring 2026");
    ASSERT_EQ(selectedTerm.getStartDate(), std::chrono::year_month_day{2026y/1/2});
    ASSERT_EQ(selectedTerm.getEndDate(), std::chrono::year_month_day{2026y/5/24});
    ASSERT_TRUE(selectedTerm.getActive());
}

TEST_F(TermControllerTest, TermListGetter) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    controller.addTerm("Spring 2026", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/24}, true);

    const std::unordered_map<std::string, Term> &listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 2);

    // check if both added terms are in the list
    std::string id1 = controller.getTermId("Fall 2025");
    auto it1 = listOfTerms.find(id1);
    ASSERT_TRUE(it1 != listOfTerms.end());

    std::string id2 = controller.getTermId("Spring 2026");
    auto it2 = listOfTerms.find(id2);
    ASSERT_TRUE(it2 != listOfTerms.end());
}

TEST_F(TermControllerTest, TermIdGetter) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    controller.addTerm("Spring 2026", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/24}, true);

    std::string id = controller.getTermId("Fall 2025");

    // find and replace UUID
    std::regex uuidRegex("[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12}");
    id = std::regex_replace(id, uuidRegex, "<UUID>");

    // if the UUID is in the correct format, it should match this
    ASSERT_EQ(id, "<UUID>");
}

// ====================================
// FUNCTION SMOKE TESTS
// ====================================

TEST_F(TermControllerTest, AddTerm) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);

    const Term& selectedTerm = controller.getTerm("Fall 2025");
    ASSERT_EQ(selectedTerm.getTitle(), "Fall 2025");
    ASSERT_EQ(selectedTerm.getStartDate(), std::chrono::year_month_day{2025y/8/15});
    ASSERT_EQ(selectedTerm.getEndDate(), std::chrono::year_month_day{2025y/12/17});
    ASSERT_FALSE(selectedTerm.getActive());
}

TEST_F(TermControllerTest, EditTitle) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);

    std::string id = controller.getTermId("Fall 2025");
    controller.editTitle(id, "Winter 2026");

    // check that title has been edited and title -> id mapping is correct
    const Term& selectedTerm = controller.getTerm("Winter 2026");
    ASSERT_EQ(selectedTerm.getTitle(), "Winter 2026");
    ASSERT_EQ(controller.getTermId("Winter 2026"), id);
}

TEST_F(TermControllerTest, EditStartDate) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    
    std::string id = controller.getTermId("Fall 2025");
    controller.editStartDate(id, std::chrono::year_month_day{2025y/8/20});

    const Term& selectedTerm = controller.getTerm("Fall 2025");
    ASSERT_EQ(selectedTerm.getStartDate(), std::chrono::year_month_day{2025y/8/20});
}

TEST_F(TermControllerTest, EditEndDate) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    
    std::string id = controller.getTermId("Fall 2025");
    controller.editEndDate(id, std::chrono::year_month_day{2025y/12/20});

    const Term& selectedTerm = controller.getTerm("Fall 2025");
    ASSERT_EQ(selectedTerm.getEndDate(), std::chrono::year_month_day{2025y/12/20});
}

TEST_F(TermControllerTest, EditActive) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    
    std::string id = controller.getTermId("Fall 2025");
    controller.editActive(id, true);

    const Term& selectedTerm = controller.getTerm("Fall 2025");
    ASSERT_TRUE(selectedTerm.getActive());
}

TEST_F(TermControllerTest, RemoveTerm) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    controller.addTerm("Spring 2026", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/24}, true);

    controller.removeTerm("Fall 2025");

    const std::unordered_map<std::string, Term> &listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 1);

    // check that this term is no longer in the list by inspecting titles directly
    bool foundRemovedItem = false;
    for (const auto &entry : listOfTerms) {
        if (entry.second.getTitle() == "Fall 2025") {
            foundRemovedItem = true;
            break;
        }
    }
    ASSERT_FALSE(foundRemovedItem);

    std::string id2 = controller.getTermId("Spring 2026");
    auto it2 = listOfTerms.find(id2);
    ASSERT_TRUE(it2 != listOfTerms.end());
}

// ====================================
// GETTER EDGE CASES
// ====================================

TEST_F(TermControllerTest, TermGetterNotFound) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    controller.addTerm("Spring 2026", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/24}, true);

    // out of range error since term cannot be found
    ASSERT_THROW(controller.getTerm("Fall 2026"), std::out_of_range);
}

TEST_F(TermControllerTest, TermListGetterEmpty) {
    const std::unordered_map<std::string, Term> &listOfTerms = controller.getTermList();
    ASSERT_EQ(listOfTerms.size(), 0);
}

TEST_F(TermControllerTest, TermIdGetterNotFound) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    controller.addTerm("Spring 2026", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/24}, true);

    // out of range error since term cannot be found
    ASSERT_THROW(controller.getTermId("Fall 2026"), std::out_of_range);
}

// ====================================
// FUNCTION EDGE CASES
// ====================================

TEST_F(TermControllerTest, AddTermAlreadyExists) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    // logic error since term already exists
    ASSERT_THROW(controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false), std::logic_error);
}

TEST_F(TermControllerTest, EditTitleAlreadyExists) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    std::string id = controller.getTermId("Fall 2025");
    
    // logic error since term already exists (title not changed)
    ASSERT_THROW(controller.editTitle(id, "Fall 2025"), std::logic_error);
}

TEST_F(TermControllerTest, EditTitleAlreadyExistsDifferentCase) {
    controller.addTerm("FALL 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    std::string id = controller.getTermId("FALL 2025");
    
    // logic error since term already exists (title not changed)
    ASSERT_THROW(controller.editTitle(id, "fall 2025"), std::logic_error);
}

TEST_F(TermControllerTest, EditTitleEmpty) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    std::string id = controller.getTermId("Fall 2025");
    
    // invalid argument since title is empty
    ASSERT_THROW(controller.editTitle(id, ""), std::invalid_argument);
}

TEST_F(TermControllerTest, EditStartDateInvalid) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    std::string id = controller.getTermId("Fall 2025");

    // invalid argument since start date does not exist
    ASSERT_THROW(controller.editStartDate(id, std::chrono::year_month_day{2026y/2/31}), std::invalid_argument);
}

TEST_F(TermControllerTest, EditEndDateInvalid) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    std::string id = controller.getTermId("Fall 2025");

    // invalid argument since end date does not exist
    ASSERT_THROW(controller.editEndDate(id, std::chrono::year_month_day{2026y/2/31}), std::invalid_argument);
}

TEST_F(TermControllerTest, RemoveTermNotFound) {
    controller.addTerm("Fall 2025", std::chrono::year_month_day{2025y/8/15}, std::chrono::year_month_day{2025y/12/17}, false);
    controller.addTerm("Spring 2026", std::chrono::year_month_day{2026y/1/2}, std::chrono::year_month_day{2026y/5/24}, true);

    // out of range since term cannot be found
    ASSERT_THROW(controller.removeTerm("Fall 2026"), std::out_of_range);
}