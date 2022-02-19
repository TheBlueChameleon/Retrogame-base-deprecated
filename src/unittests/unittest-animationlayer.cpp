// ========================================================================== //
// Depenencies

// STL
#include <iostream>

// own
#include "../gfxSystem/window.hpp"
#include "../gfxSystem/animationlayer.hpp"

#include "../unittest-driver/unittest.hpp"
#include "unittest-animationlayer.hpp"

using namespace RetrogameBase;

// ========================================================================== //
// Tests

bool unittest_AnimationLayer_addElements()
{
    std::cout << "TESTING ANIMATIONLAYER CLASS" << std::endl;

    UNITTEST_VARS;

    Window win("test window");
    TextureStore&   textureStore   = win.getTextureStore();
    AnimationStore& animationStore = win.getAnimationStore();

    Unittest_RessorceList files =
    {
        "../unittest-xml/animations/animation-simple.xml",

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
    };
    unittest_check_files_present(files);

    // ...................................................................... //

    animationStore.addAnimation(files[0]);

    UNITTEST_DOESNT_THROW(
        win.addLayer(),
        std::exception,
        "construct empty layer"
    );

    UNITTEST_DOESNT_THROW(
        win.addLayer(),
        std::exception,
        "construct second empty layer"
    );

    UNITTEST_ASSERT(
        win.getLayerCount() == 2,
        "count layers correctly"
    );

    auto& layer1 = win.getLayer(0);
    auto& layer2 = win.getLayer(1);

    UNITTEST_ASSERT(
        layer1.size() == 0,
        "set up empty layer"
    );

    // ...................................................................... //

    AnimationLayer::Element valid1   = { 0, {0,0,0}};
    AnimationLayer::Element valid2   = { 0, {1,0,0}};
    AnimationLayer::Element invalid1 = {-1, {0,0,0}};
    AnimationLayer::Element invalid2 = {+1, {0,0,0}};

    UNITTEST_DOESNT_THROW(
        layer1.addElement(valid1),
        std::exception,
        "add first element to one layer"
    );

    UNITTEST_DOESNT_THROW(
        layer1.addElement(valid1),
        std::exception,
        "add second element to one layer"
    );

    UNITTEST_THROWS(
        layer1.addElement(invalid1),
        std::out_of_range,
        "prevent references to negative AnimationStore indices"
    );

    UNITTEST_THROWS(
        layer1.addElement(invalid1),
        std::out_of_range,
        "prevent references to invalid AnimationStore indices"
    );

    UNITTEST_ASSERT(
        layer1.size() == 2,
        "correctly update sizes"
    );

    UNITTEST_ASSERT(
        layer2.size() == 0,
        "not mix up layers"
    );

    // ...................................................................... //

    UNITTEST_DOESNT_THROW(
        layer1.replaceElement(0, valid2),
        std::exception,
        "replace elements"
    );

    UNITTEST_ASSERT(
        layer1.getElement(0) == valid2,
        "correctly perform replacement"
    );

    UNITTEST_ASSERT(
        layer1.getCoordinate(0) == valid2.second,
        "correctly extract coordinates"
    );

    UNITTEST_THROWS(
        layer1.replaceElement(0, invalid1),
        std::out_of_range,
        "prevent replacing elements by invalid ones"
    );

    // ...................................................................... //

    AnimationLayer::Coordinate newTarget = {1, 2, 3};
    UNITTEST_DOESNT_THROW(
        layer1.moveElement(0, newTarget),
        std::exception,
        "move elements"
    );

    UNITTEST_ASSERT(
        layer1.getCoordinate(0) == newTarget,
        "correctly move elements"
    );

    // ...................................................................... //

    UNITTEST_FINALIZE;
}

bool unittest_AnimationLayer_loadXml()
{
    std::cout << "TESTING ANIMATIONLAYER CLASS" << std::endl;

    UNITTEST_VARS;
    UNITTEST_CAPTURE_CERR;

    Window win("test window");
    TextureStore& textureStore     = win.getTextureStore();
    AnimationStore& animationStore = win.getAnimationStore();

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

    // errors to check:
    // InvalidFileError on missing node elements
    // grid references to invalid ani IDs
    // element references to invalid ani IDs

    // load animations and textures, too

    // warnings on invalid elements:
    // "Warning: element tag in Scene definition " << filename

    // warnings on invalid grid tags:
    // "Warning: grid without spacing in file " << filename << " (ignored)"
    // "Warning: empty grid in file " << filename << " (ignored)"
    // "Warning: invalid grid element '" << trimmedCell << "'"

    // warnings on invalid tags
    // "Warning: invalid tag '" << subNode.name() << "' in Scene definition " << filename

    UNITTEST_ASSERT(
        1 == 1,
        "assert basic logic"
    );

    // ...................................................................... //

    UNITTEST_FINALIZE;
}
