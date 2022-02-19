// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <tuple>

// own
#include "../base/globals.hpp"

#include "../unittest-driver/unittest.hpp"
#include "unittest-globals.hpp"

using namespace RetrogameBase;

// ========================================================================== //
// tests

bool unittest_globals_loadFont()
{
    std::cout << "TESTING FONT ENGINE" << std::endl;

    UNITTEST_VARS;

    // ...................................................................... //

    UNITTEST_ASSERT(
        fonts.size() == 3,
        "prepare three fonts by default"
    );

    for (auto & [name, fontPtr] : fonts)
    {
        UNITTEST_ASSERT(
            static_cast<bool>(fontPtr),
            "load font with alias '" << name << "' correctly"
        );
    }

    UNITTEST_THROWS(
        loadFont("alias", "nonexistent file", 12),
        FileNotFoundError,
        "throw on loading nonexistent file"
    );

    UNITTEST_THROWS(
        loadFont("fixed-small", "../font/FreeMono.ttf", 8),
        MemoryManagementError,
        "throw on loading font again"
    );

    // ...................................................................... //

    UNITTEST_FINALIZE;
}

bool unittest_globals_isInteger()
{
    std::cout << "TESTING ISINTEGER FUNCTION" << std::endl;

    std::vector<std::pair<std::string, bool>> testData =
    {
        {"0", true}, {"+0", true}, {"-0", true},
        {"1", true}, {"+1", true}, {"-1", true},
        {"0.", false}, {"+0.", false}, {"-0.", false},
        {"ABC", false}, {"", false},
        {" 0", false}, {"0 ", false},
    };

    UNITTEST_VARS;

    // ...................................................................... //

    for (auto& [testString, testResult] : testData)
    {
        UNITTEST_ASSERT(
            isInteger(testString) == testResult,
            "correctly identify '" << testString << "' as "
            << (testResult ? "integer" : "non-integer")
        );
    }

    // ...................................................................... //

    UNITTEST_FINALIZE;
}

bool unittest_globals_split_nonowning()
{
    std::cout << "TESTING SPLIT_NONOWNING FUNCTION" << std::endl;

    UNITTEST_VARS;

    using testInput_t  = std::pair<std::string, std::string>;
    using testOutput_t = std::vector<std::string_view>;

    std::vector<std::pair<testInput_t, testOutput_t>> testData =
    {
        {{"", ""}, {}}, {{"", "anything"}, {}}, {{"ABC", ""}, {"ABC"}},
        {{"A,B,C", ","}, {"A", "B", "C"}},
        {{"A<>B<>C", "<>"}, {"A", "B", "C"}},
        {{"A<>B<C", "<>"}, {"A", "B<C"}}, {{"A<>B>C", "<>"}, {"A", "B>C"}},

    };

    // ...................................................................... //

    for (auto& [testInput, testOutput] : testData)
    {
        auto& [toSplit, splitAt] = testInput;
        UNITTEST_ASSERT(
            split_nonowning(toSplit, splitAt) == testOutput,
            "correctly split '" << toSplit << "' at '" << splitAt << "'"
        );
    }

    // ...................................................................... //

    UNITTEST_FINALIZE;
}

bool unittest_globals_trim_nonowning()
{
    std::cout << "TESTING TRIM_NONOWNING FUNCTION" << std::endl;

    UNITTEST_VARS;

    using testInput_t  = std::pair<std::string, std::string>;
    using testOutput_t = std::string_view;

    std::vector<std::pair<testInput_t, testOutput_t>> testData =
    {
        {{"", ""}, ""},
        {{"", "anything"}, ""},
        {{"ABC", ""}, "ABC"},
        {{" ABC ", " "}, "ABC"},
        {{"abcABC", "cba"}, "ABC"},
    };

    // ...................................................................... //

    for (auto& [testInput, testOutput] : testData)
    {
        auto& [toTrim, trimFrom] = testInput;
        UNITTEST_ASSERT(
            trim_nonowning(toTrim, trimFrom) == testOutput,
            "correctly trim '" << toTrim << "' from '" << trimFrom << "'"
        );
    }

    // ...................................................................... //

    UNITTEST_FINALIZE;
}
