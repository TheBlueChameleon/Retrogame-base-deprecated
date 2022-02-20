// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <stdexcept>

// own
#include "../gfxSystem/window.hpp"
#include "../gfxSystem/animationlayerstore.hpp"

#include "../unittest-driver/unittest.hpp"
#include "unittest-animationlayerstore.hpp"

using namespace RetrogameBase;

// ========================================================================== //
// tests


bool unittest_AnimationLayerStore_addReset()
{
    std::cout << "TESTING WINDOW STORE INTERFACE" << std::endl;

    UNITTEST_VARS;
    UNITTEST_CAPTURE_CERR;

    Window win("test window");
    TextureStore& textureStore               = win.getTextureStore();
    AnimationStore& animationStore           = win.getAnimationStore();
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

    // ...................................................................... //

    {
        auto [layerID, layer] = animationLayerStore.addLayer(files[0]);     // scope to protect variable layer which will be destroyed later on.
        UNITTEST_CLEAR_CERR;

        UNITTEST_ASSERT(
            layerID == 0,
            "instantiate an AnimationLayer"
        );

        UNITTEST_ASSERT(
            animationLayerStore.size() == 1,
            "correctly set up layers memory"
        );

        UNITTEST_ASSERT(
            animationStore.size() == 4,
            "correctly load test animations"
        );

        UNITTEST_ASSERT(
            textureStore.size() == 15,
            "correctly load associated test frames"
        );

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
    }

    // ...................................................................... //

    {
        auto [layerID, layer] = animationLayerStore.addLayer(files[0]);

        UNITTEST_ASSERT(
            layerID == 0,
            "re-instantiate an AnimationLayer"
        );

        UNITTEST_ASSERT(
            layer.size() == 54,
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
    }

    // ...................................................................... //

    UNITTEST_CLEAR_CERR;
    UNITTEST_FINALIZE;
}
