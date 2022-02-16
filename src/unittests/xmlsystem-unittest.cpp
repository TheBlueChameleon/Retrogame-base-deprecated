// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <stdexcept>

#include <cstring>

#include <filesystem>
namespace fs = std::filesystem;

// Pugi
#include "../xmlSystem/pugixml.hpp"

// own
#include "unittest-macros.hpp"
#include "../xmlSystem/xmlwrapper.hpp"
#include "xmlsystem-unittest.hpp"

using namespace RetrogameBase;

bool unittest_xmlSystem_load()
{
    std::cout << "TESTING XML LOAD CONVENIENCE FUNCTION" << std::endl;

    UNITTEST_VARS;
    auto dir_invalidFiles = "../unittest-xml/xml-invalid/";
    auto dir_animations   = "../unittest-xml/animations/";

    // ...................................................................... //

    UNITTEST_ASSERT(
        !std::strcmp(projectName, "Retrogame-base"),
        "fetch project name from definitions"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        codeVersion_major == 1,
        "fetch project version major"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        codeVersion_minor == 1,
        "fetch project version minor"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        fs::is_directory(dir_invalidFiles),
        "find directory with invalid files"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        !fs::is_empty(dir_invalidFiles),
        "find files in directory with invalid files"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        fs::is_directory(dir_animations),
        "find directory with valid files"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        !fs::is_empty(dir_animations),
        "find files in directory with valid files"
    );
    UNITTEST_CRITICAL_BARRIER;

    // ...................................................................... //

    UNITTEST_THROWS(
        XmlLoad("nonexistent file", ""),
        std::runtime_error,
        "throw on loading nonexistent file"
    );

    fs::directory_iterator invalidFiles(dir_invalidFiles);
    for (auto& file : invalidFiles)
    {
        const std::string& filename = file.path().string();
        UNITTEST_THROWS(
            XmlLoad(filename, ""),
            std::runtime_error,
            "throw on loading file '" << filename << "'"
        );
    }

    // ...................................................................... //

    fs::directory_iterator animations(dir_animations);
    for (auto& file : animations)
    {
        const std::string& filename = file.path().string();
        UNITTEST_DOESNT_THROW(
            XmlLoad(filename, "animation"),
            std::runtime_error,
            "load valid file '" << filename << "'"
        );

        UNITTEST_THROWS(
            XmlLoad(filename, "wrong content type"),
            std::runtime_error,
            "throw on loading file '" << filename << "' with mismatched expected content type"
        );
    }

    // ...................................................................... //

    UNITTEST_FINALIZE;
}

bool unittest_xmlSystem_XmlExtractSimpleGroup()
{
    std::cout << "TESTING XML EXTRACT SIMPLE GROUP" << std::endl;

    UNITTEST_VARS;
    auto testfile = "../unittest-xml/animations/animation-pure.xml";
    XmlSimpleGroup expected =
    {
        {
            "frame", {
                {"file", "nonexistent file will not throw an error due to implicit repeat=0"},
                {"repeat", ""}
            }
        }, {
            "frame", {
                {"file", "../unittest-gfx/frame06.png"},
                {"file", "this should be ignored"}
            }
        }, {
            "new", {
                {"meaning", "This is to test whether the system is tolerant towards unforseen tags in the list"}
            }
        }, {
            "frame", {
                {"file", "../unittest-gfx/frame01.png"}
            }
        }, {
            "frame", {
                {"file", "../unittest-gfx/frame02.png"}
            }
        }, {
            "frame", {
                {"file", "../unittest-gfx/frame03.png"}
            }
        }, {
            "frame", {
                {"file", "../unittest-gfx/frame04.png"}
            }
        }, {
            "frame", {
                {"file", "../unittest-gfx/frame05.png"}
            }
        }, {
            "frame", {
                {"file", "../unittest-gfx/frame07.png"}
            }
        }, {
            "frame", {
                {"file", "../unittest-gfx/frame08.png"}
            }
        }, {
            "frame", {
                {"file", "../unittest-gfx/frame09.png"}
            }
        }, {
            "frame",{
                {"file", "../unittest-gfx/frame10.png"},
                {"repeat", "10"}
            }
        }
    };

    // ...................................................................... //

    UNITTEST_ASSERT(
        !std::strcmp(projectName, "Retrogame-base"),
        "fetch project name from definitions"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        codeVersion_major == 1,
        "fetch project version major"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        codeVersion_minor == 1,
        "fetch project version minor"
    );
    UNITTEST_CRITICAL_BARRIER;

    auto doc = XmlLoad(testfile);
    auto root = doc.child("project");
    UNITTEST_ASSERT(
        !root.empty(),
        "find the root node in the testfile"
    );
    UNITTEST_CRITICAL_BARRIER;

    auto nodeAnimation = root.child("animation");
    UNITTEST_ASSERT(
        !nodeAnimation.empty(),
        "find the animation node in the testfile"
    );
    UNITTEST_CRITICAL_BARRIER;

    // ...................................................................... //

    auto attributeList= XmlExtractSimpleGroup(nodeAnimation);

    UNITTEST_ASSERT(
        !attributeList.empty(),
        "load any elements at all"
    );

    UNITTEST_ASSERT(
        attributeList == expected,
        "reconstruct expected structure"
    );

    // ...................................................................... //

    UNITTEST_FINALIZE;
}
