// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <stdexcept>

// own
#include "../base/globals.hpp"
#include "../gfxSystem/window.hpp"

#include "../unittest-driver/unittest.hpp"
#include "unittest-window.hpp"

using namespace RetrogameBase;

// ========================================================================== //
// tests

bool unittest_Window_CTor()
{
    std::cout << "TESTING WINDOW CTORS" << std::endl;

    UNITTEST_VARS;

    // ...................................................................... //

    UNITTEST_DOESNT_THROW(
        Window win("valid window"),
        std::exception,
        "set up default window"
    );

    UNITTEST_DOESNT_THROW(
        Window win(""),
        std::exception,
        "set up window with empty string as title"
    );

    UNITTEST_DOESNT_THROW(
        Window win(nullptr),
        std::exception,
        "set up window with nullptr as title"
    );

    // ...................................................................... //

    UNITTEST_DOESNT_THROW(
        Window win("null size", 0, 0),
        std::exception,
        "set up window with null size"
    );

    UNITTEST_DOESNT_THROW(
        Window win("negative size", -1, -1),
        std::exception,
        "set up window with negative size"
    );

    // ...................................................................... //

    UNITTEST_THROWS(
        Window win("null size", 80, 60, -1),
        SdlInternalError,
        "throw on set up window with all flags"
    );

    // ...................................................................... //

    UNITTEST_FINALIZE;
}

bool unittest_Window_Stores()
{
    std::cout << "TESTING WINDOW STORE INTERFACE" << std::endl;

    UNITTEST_VARS;
    UNITTEST_CAPTURE_CERR;

    Window win("test window");
    TextureStore&        textureStore        = win.getTextureStore();
    AnimationStore&      animationStore      = win.getAnimationStore();
    AnimationLayerStore& animationLayerStore = win.getAnimationLayerStore();

    Unittest_RessorceList files =
    {
        "../unittest-xml/animationlayers/scene.xml",

        "../unittest-xml/animations/animation-pure.xml",
        "../unittest-xml/animations/animation-simple.xml",
        "../unittest-xml/animations/animation-valid-deprecated.xml",

        "../unittest-gfx/frame01.png",
        "../unittest-gfx/frame02.png",
        "../unittest-gfx/frame03.png",
        "../unittest-gfx/frame04.png",
        "../unittest-gfx/frame05.png",
        "../unittest-gfx/frame06.png",
        "../unittest-gfx/frame07.png",
        "../unittest-gfx/frame08.png",
        "../unittest-gfx/frame09.png",
        "../unittest-gfx/frame10.png",
        "../unittest-gfx/sea01.png",
        "../unittest-gfx/sea02.png",
        "../unittest-gfx/sea03.png",
        "../unittest-gfx/sea04.png",
        "../unittest-gfx/sea05.png",
    };
    unittest_check_files_present(files);

    animationLayerStore.addLayer();
    animationLayerStore.addLayer(files[0]);
    UNITTEST_CLEAR_CERR;

    UNITTEST_ASSERT(
        animationLayerStore.size() == 2,
        "instantiate two AnimationLayers"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        animationLayerStore.getLayer(1).size() == 54,
        "load data into layers from xml"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        animationStore.size() == 4,
        "correctly load test animations"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        textureStore.size() == 15,
        "correctly load associated test frames"
    );
    UNITTEST_CRITICAL_BARRIER;

    // ...................................................................... //

    UNITTEST_DOESNT_THROW(
        win.resetStores(Window::ResetStoresDepth::Layers),              /* note that this does invoke the DTor on layer! */
        std::exception,
        "perform Window::ResetStoresDepth::Layers"
    );

    UNITTEST_ASSERT(
        animationLayerStore.size() == 0,
        "actually clear layers"
    );

    UNITTEST_ASSERT(
        animationStore.size() == 4,
        "preserve animations"
    );

    UNITTEST_ASSERT(
        textureStore.size() == 15,
        "preserve associated test frames"
    );

    // ...................................................................... //

    animationLayerStore.addLayer();
    animationLayerStore.addLayer(files[0]);
    UNITTEST_CLEAR_CERR;

    UNITTEST_ASSERT(
        animationLayerStore.size() == 2,
        "re-instantiate two AnimationLayers"
    );
    UNITTEST_CRITICAL_BARRIER;

    // ...................................................................... //

    UNITTEST_ASSERT(
        animationLayerStore.getLayer(1).size() == 54,
        "restore data after reset"
    );

    UNITTEST_DOESNT_THROW(
        win.resetStores(Window::ResetStoresDepth::Animations),              /* note that this does invoke the DTor on layer! */
        std::exception,
        "perform Window::ResetStoresDepth::Animations"
    );

    UNITTEST_ASSERT(
        animationLayerStore.size() == 0,
        "actually clear layers"
    );

    UNITTEST_ASSERT(
        animationStore.size() == 0,
        "actually clear animations"
    );

    UNITTEST_ASSERT(
        textureStore.size() == 15,
        "preserve associated test frames"
    );

    // ...................................................................... //

    animationLayerStore.addLayer();
    animationLayerStore.addLayer(files[0]);
    UNITTEST_CLEAR_CERR;

    UNITTEST_ASSERT(
        animationLayerStore.size() == 2,
        "re-instantiate two AnimationLayers"
    );
    UNITTEST_CRITICAL_BARRIER;

    UNITTEST_ASSERT(
        animationStore.size() == 4,
        "re-instantiate two AnimationStore"
    );
    UNITTEST_CRITICAL_BARRIER;

    // ...................................................................... //

    UNITTEST_DOESNT_THROW(
        win.resetStores(Window::ResetStoresDepth::Textures),              /* note that this does invoke the DTor on layer! */
        std::exception,
        "perform Window::ResetStoresDepth::Textures"
    );

    UNITTEST_ASSERT(
        animationLayerStore.size() == 0,
        "actually clear layers"
    );

    UNITTEST_ASSERT(
        animationStore.size() == 0,
        "actually clear animations"
    );

    UNITTEST_ASSERT(
        textureStore.size() == 0,
        "actually clear associated test frames"
    );

    // ...................................................................... //

    UNITTEST_CLEAR_CERR;
    UNITTEST_FINALIZE;
}
