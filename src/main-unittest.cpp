// ========================================================================== //
// Depenencies

// STL
#include <iostream>

#include <vector>
#include <string>
#include <functional>

// own
#include "globals.hpp"
#include "unittests/globals-unittest.hpp"
#include "unittests/window-unittest.hpp"
#include "unittests/texturestore-unittest.hpp"
#include "unittests/animation-unittest.hpp"
#include "unittests/xmlsystem-unittest.hpp"

// ========================================================================== //
// convenience macro

#define ADD_UNITTEST(func) {unittests.emplace_back(std::make_pair(#func, func));}

// ========================================================================== //

int main()
{
    RetrogameBase::initAll();
    std::cout << "UNIT TEST MAIN" << std::endl;

    std::vector<std::pair<std::string, std::function<bool()>>> unittests;
    std::vector<std::string> failedTests;

    // ...................................................................... //

    std::cout << "REGISTERING UNIT TESTS ... " << std::flush;

//    ADD_UNITTEST(unittest_loadFont);
//    ADD_UNITTEST(unittest_Window_CTor);
//    ADD_UNITTEST(unittest_Window_Stores);
//    ADD_UNITTEST(unittest_TextureStore_addReset);
//    ADD_UNITTEST(unittest_Animation_addReset);
    ADD_UNITTEST(unittest_xmlSystem_load);
    ADD_UNITTEST(unittest_xmlSystem_XmlExtractSimpleGroup);

    std::cout << "DONE" << std::endl << std::endl;

    // ...................................................................... //

    std::cout << "ABOUT TO RUN UNIT TESTS" << std::endl;
    for (auto & [name, func] : unittests)
    {
        std::cout << "### STARTING TEST '" << name << "' ..." << std::endl;
        if (! func())
        {
            failedTests.push_back(name);
            std::cout << "~~~ FAILED!" << std::endl;
        }
        else
        {
            std::cout << "~~~ PASSED!" << std::endl;
        }
    }
    std::cout << "ALL REGISTERED UNIT TESTS DONE" << std::endl << std::endl;

    // ...................................................................... //

    std::cout << "UNIT TESTS SUMMARY" << std::endl;
    std::cout << "Passed " << unittests.size() - failedTests.size() << "/" << unittests.size() << " tests"  << std::endl;
    if (!failedTests.empty())
    {
        std::cout << "Failed Tests:" << std::endl;
        for (auto& name : failedTests)
        {
            std::cout << "~~~ " << name << std::endl;
        }
    }
    return 0;
}
