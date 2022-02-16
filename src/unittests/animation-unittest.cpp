// ========================================================================== //
// Depenencies

// STL
#include <stdexcept>

#include <iostream>
#include <sstream>

#include <filesystem>
namespace fs = std::filesystem;

// own
#include "unittest-macros.hpp"
#include "../gfxSystem/window.hpp"
#include "../gfxSystem/texturestore.hpp"
#include "../gfxSystem/animation.hpp"
#include "../xmlSystem/xmlwrapper.hpp"
#include "animation-unittest.hpp"

using namespace RetrogameBase;

bool unittest_Animation_addReset()
{
    std::cout << "TESTING ANIMATION CLASS: ADD AND RESET METHODS" << std::endl;

    UNITTEST_VARS;
    size_t ID;

    constexpr auto testfile_1 = "../unittest-gfx/sea01.png";
    constexpr auto testfile_2 = "../unittest-gfx/sea02.png";
    constexpr auto testfile_X = "../unittest-gfx/square.jpg";

    constexpr auto dimensions_1 = std::make_pair(50, 50);
    constexpr auto dimensions_X = std::make_pair(10, 10);

    std::vector<size_t> frameIDs = {0, 1, 0, 1, 1, 1};

    Window win("test window");
    TextureStore& tex = win.getTextureStore();
    Animation ani(win);

    // ...................................................................... //

    UNITTEST_ASSERT(
        ani.size() == 0,
        "set up empty animation store by default"
    );

    ani.reset();
    UNITTEST_ASSERT(
        ani.size() == 0,
        "reset empty animation store"
    );

    // ...................................................................... //

    UNITTEST_THROWS(
        ani.addFrame(0, 1),
        std::out_of_range,
        "throw when adding frames while underlying texture store is empty"
    );

    UNITTEST_THROWS(
        ani.addFrame(0, 0),
        std::out_of_range,
        "throw when adding zero repetition frames while underlying texture store is empty"
    );

    // ...................................................................... //

    UNITTEST_ASSERT(
        fs::exists(testfile_1) && fs::exists(testfile_2),
        "find test files on disk"
    );
    UNITTEST_CRITICAL_BARRIER;

    tex.addFrame(testfile_1);
    tex.addFrame(testfile_2);
    tex.addFrame(testfile_X);

    // ...................................................................... //

    UNITTEST_DOESNT_THROW(
        ani.addFrame(0, 1),
        std::exception,
        "add frames from filled store"
    );

    UNITTEST_DOESNT_THROW(
        ani.addFrame(1, 1),
        std::exception,
        "add frames from filled store"
    );

    UNITTEST_THROWS(
        ani.addFrame(3, 1),
        std::out_of_range,
        "throw when adding non-existing frames from filled store"
    );

    UNITTEST_THROWS(
        ani.addFrame(2, 1),
        std::runtime_error,
        "throw when adding frame with non-matching dimension frames from filled store"
    );

    UNITTEST_DOESNT_THROW(
        ani.addFrame(0, 1),
        std::exception,
        "repeatedly add same frame from filled store"
    );

    UNITTEST_DOESNT_THROW(
        ani.addFrame(0, 0),
        std::exception,
        "add frames from filled store with zero repetition"
    );

    UNITTEST_DOESNT_THROW(
        ani.addFrame(1, 3),
        std::exception,
        "add frames from filled store with repetition"
    );

    UNITTEST_ASSERT(
        ani.size() == 6,
        "correctly report frame count after multiple added frames"
    );

    UNITTEST_ASSERT(
        ani.getFrameIDs() == frameIDs,
        "correctly report frame IDs after multiple added frames"
    );

    // ...................................................................... //

    UNITTEST_ASSERT(
        ani.getDimension() == dimensions_1,
        "correctly report frame dimension after multiple added frames"
    );

    // ...................................................................... //

    for (auto i = 0u; i < ani.size(); ++i)
    {
        UNITTEST_ASSERT(
            ani.getCurrentPhase() == i,
            "correctly count to phase #" << i
        );

        ID = ani.getCurrentFrameID();
        UNITTEST_ASSERT(
            ID == frameIDs[i],
            "correctly report frame ID #" << i
        );

        UNITTEST_ASSERT(
            ani.getCurrentFrameTexture() == tex.getTexture(ID),
            "correctly report texture pointer #" << i
        );

        ani.advanceFrame();
    }

    UNITTEST_ASSERT(
        ani.getCurrentPhase() == 0,
        "correctly cycle back phase #0"
    );

    // ...................................................................... //

    ani.reset();
    UNITTEST_ASSERT(
        ani.size() == 0,
        "reset filled animation store"
    );

    // ...................................................................... //

    UNITTEST_FINALIZE;
}

bool unittest_Animation_loadXml()
{
    std::cout << "TESTING ANIMATION CLASS: LOAD XML METHOD" << std::endl;

    UNITTEST_VARS;

    Window win("Test Window");
    TextureStore& tex = win.getTextureStore();
    Animation ani(win);

    const auto testfile    = "../unittest-xml/animations/animation-pure.xml";
    const auto dir_invalidFiles = "../unittest-xml/animations-invalid";

    const std::vector<size_t> expectedFrameIDs = {0,1,2,3,4,5,6,7,8,9,9,9,9,9,9,9,9,9,9};
    const std::vector<std::string> expectedFilenames =
    {
        "../unittest-gfx/frame06.png",

        "../unittest-gfx/frame01.png",
        "../unittest-gfx/frame02.png",
        "../unittest-gfx/frame03.png",
        "../unittest-gfx/frame04.png",
        "../unittest-gfx/frame05.png",
        "../unittest-gfx/frame07.png",
        "../unittest-gfx/frame08.png",
        "../unittest-gfx/frame09.png",

        "../unittest-gfx/frame10.png",
        "../unittest-gfx/frame10.png",
        "../unittest-gfx/frame10.png",
        "../unittest-gfx/frame10.png",
        "../unittest-gfx/frame10.png",
        "../unittest-gfx/frame10.png",
        "../unittest-gfx/frame10.png",
        "../unittest-gfx/frame10.png",
        "../unittest-gfx/frame10.png",
        "../unittest-gfx/frame10.png"
    };

    // redirect stderr to local buffer to check warings
    std::stringstream cerrBuffer;
    std::streambuf* originalCerr = std::cerr.rdbuf(cerrBuffer.rdbuf());

    std::string expectedWarnings = "Warning: duplicate definition of filename\n"
                                   "  Animation Definition    : ../unittest-xml/animations/animation-pure.xml\n"
                                   "  Previous Frame Reference: ../unittest-gfx/frame06.png\n"
                                   "  New Frame Reference     : this should be ignored (ignored)\n"
                                   "Warning: invalid tag in Animation Definition ../unittest-xml/animations/animation-pure.xml\n";

    // ...................................................................... //

    UNITTEST_ASSERT(
        fs::exists(testfile),
        "find testfile"
    );
    UNITTEST_CRITICAL_BARRIER;

    auto doc = XmlLoad(testfile);
    auto root = doc.child("project");
    auto nodeAnimation = root.child("animation");
    UNITTEST_ASSERT(
        !nodeAnimation.empty(),
        "find the animation node in the testfile"
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

    // ...................................................................... //

    UNITTEST_DOESNT_THROW(
        ani.loadXML(testfile),
        std::exception,
        "load without throw"
    );

    UNITTEST_ASSERT(
        cerrBuffer.str() == expectedWarnings,
        "utter expected warnings"
    );

    UNITTEST_ASSERT(
        ani.size() == expectedFrameIDs.size(),
        "load correct number of frames"
    );

    UNITTEST_ASSERT(
        ani.getFrameIDs() == expectedFrameIDs,
        "construct expected frame IDs"
    );

    for (auto i = 0u; i < expectedFrameIDs.size(); ++i)
    {
        UNITTEST_ASSERT(
            tex.getFilename(expectedFrameIDs[i]) ==expectedFilenames[i],
            "read file " << i << " in correct order"
        );
    }

    // ...................................................................... //

    fs::directory_iterator invalidFiles(dir_invalidFiles);
    for (const auto& file : invalidFiles)
    {
        const std::string& filename = file.path().string();
        UNITTEST_THROWS(
            ani.loadXML(filename),
            std::runtime_error,
            "throw on loading file '" << filename << "'"
        );
    }

    // ...................................................................... //

    std::cerr.rdbuf( originalCerr );
    UNITTEST_FINALIZE;
}
