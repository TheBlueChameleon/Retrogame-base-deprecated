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
#include "animationstore-unittest.hpp"

using namespace RetrogameBase;

bool unittest_Animationstore_addReset()
{
    UNITTEST_VARS;

    constexpr auto testfile_1 = "../unittest-xml/animations/animation-pure.xml";
    constexpr auto testfile_2 = "../unittest-xml/animations/animation-sea.xml";
    constexpr auto testfile_X = "../unittest-xml/animations-invalid/invalid-filename.xml";

    Window win("test window");
    AnimationStore aniStore(win);

    // ...................................................................... //

    UNITTEST_ASSERT(
        fs::exists(testfile_1),
        "valid testfile " << testfile_1 << " exists"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        fs::exists(testfile_2),
        "valid testfile " << testfile_2 << " exists"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        fs::exists(testfile_X),
        "invalid testfile " << testfile_X << " exists"
    );
    UNITTEST_CRITICAL_BARRIER;

    // ...................................................................... //

    UNITTEST_ASSERT(
        aniStore.size() == 0,
        "initialize empty AnimationStore"
    );

    UNITTEST_DOESNT_THROW(
        aniStore.addAnimation(testfile_1),
        std::exception,
        "load first valid file"
    );

    UNITTEST_DOESNT_THROW(
        aniStore.addAnimation(testfile_1),
        std::exception,
        "load first valid file a second time"
    );

    UNITTEST_DOESNT_THROW(
        aniStore.addAnimation(testfile_2),
        std::exception,
        "load second valid file"
    );


    UNITTEST_THROWS(
        aniStore.addAnimation(testfile_X),
        std::runtime_error,
        "throw on load of invalid file"
    );


    UNITTEST_ASSERT(
        aniStore.size() == 2,
        "prevent double loading"
    );

    // ...................................................................... //

    UNITTEST_FINALIZE;
}

bool unittest_Animationstore_advanceAll()
{
    UNITTEST_VARS;

    constexpr auto testfile_1 = "../unittest-xml/animations/animation-sea.xml";                 //  5 frames
    constexpr auto testfile_2 = "../unittest-xml/animations/animation-valid-deprecated.xml";    // 10 frames

    Window win("test window");
    AnimationStore aniStore(win);

    // ...................................................................... //

    UNITTEST_ASSERT(
        fs::exists(testfile_1),
        "valid testfile " << testfile_1 << " exists"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        fs::exists(testfile_2),
        "valid testfile " << testfile_2 << " exists"
    );
    UNITTEST_CRITICAL_BARRIER;

    // ...................................................................... //

    aniStore.addAnimation(testfile_1);
    aniStore.addAnimation(testfile_2);

    for (auto i=0u; i<aniStore.size(); ++i)
    {
        UNITTEST_ASSERT(
            aniStore.getAnimation(i).getCurrentPhase() == 0,
            "initialize all animations at phase zero"
        );
    }

    aniStore.advanceAll();

    for (auto i=0u; i<aniStore.size(); ++i)
    {
        UNITTEST_ASSERT(
            aniStore.getAnimation(i).getCurrentPhase() == 1,
            "advance animation " << i << " by one step "
        );
    }

    for (auto i=0u; i<4; ++i)
    {
        aniStore.advanceAll();
    }
    UNITTEST_ASSERT(
        aniStore.getAnimation(0).getCurrentPhase() == 0,
        "roll back animation 0"
    );
    UNITTEST_ASSERT(
        aniStore.getAnimation(1).getCurrentPhase() == 5,
        "advance animation 1"
    );

    // ...................................................................... //

    UNITTEST_FINALIZE;
}
