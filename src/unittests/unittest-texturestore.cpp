// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <stdexcept>

#include <filesystem>
namespace fs = std::filesystem;

#include <utility>

// own
#include "../gfxSystem/window.hpp"
#include "../gfxSystem/texturestore.hpp"

#include "../unittest-driver/unittest.hpp"
#include "unittest-texturestore.hpp"

using namespace RetrogameBase;

// ========================================================================== //
// tests

bool unittest_TextureStore_addReset()
{
    std::cout << "TESTING TEXTURESTORE ADD AND RESET METHODS" << std::endl;

    UNITTEST_VARS;
    size_t ID;

    constexpr auto testfile_1 = "../unittest-gfx/sea01.png";
    constexpr auto testfile_2 = "../unittest-gfx/sea02.png";

    constexpr auto dimensions_1 = std::make_pair(50, 50);
    constexpr auto dimensions_2 = std::make_pair(50, 50);

    Window win("test window");
    TextureStore& tex = win.getTextureStore();

    // ...................................................................... //

    UNITTEST_ASSERT(
        tex.size() == 0,
        "set up empty texture store by default"
    );

    tex.reset();
    UNITTEST_ASSERT(
        tex.size() == 0,
        "reset empty texture store by default"
    );

    // ...................................................................... //

    UNITTEST_ASSERT(
        fs::exists(testfile_1) && fs::exists(testfile_2),
        "find test files on disk"
    );
    UNITTEST_CRITICAL_BARRIER;

    // ...................................................................... //

    UNITTEST_DOESNT_THROW(
        ID = tex.addFrame(testfile_1),
        std::exception,
        "load existing file"
    );

    UNITTEST_ASSERT(
        ID == 0,
        "return correct index for first texture"
    );

    // ...................................................................... //

    UNITTEST_THROWS(
        ID = tex.addFrame("a file that does not exist"),
        FileNotFoundError,
        "throw std::runtime_error on loading non-existent file"
    );

    // ...................................................................... //

    UNITTEST_DOESNT_THROW(
        ID = tex.addFrame(testfile_1),
        std::exception,
        "enter addFrame on previously loaded file"
    );

    UNITTEST_ASSERT(
        ID == 0,
        "prevent actually loading same file twice"
    );

    // ...................................................................... //

    UNITTEST_DOESNT_THROW(
        ID = tex.addFrame(testfile_2),
        std::exception,
        "load a second file"
    );

    UNITTEST_ASSERT(
        ID == 1,
        "load second file"
    );

    UNITTEST_ASSERT(
        tex.size() == 2,
        "record size correctly after adding textures"
    );

    // ...................................................................... //

    UNITTEST_ASSERT(
        (tex.getTexture(0) != nullptr) && (tex.getTexture(1) != nullptr),
        "obtain valid texture ptrs"
    );

    UNITTEST_ASSERT(
        (tex.getTextureDimension(0) == dimensions_1) && (tex.getTextureDimension(1) == dimensions_2),
        "retrieve correct dimensions"
    );

    UNITTEST_ASSERT(
        (tex.getFilename(0) == testfile_1) && (tex.getFilename(1) == testfile_2),
        "record filenames"
    );

    UNITTEST_ASSERT(
        (tex.findByFilename(testfile_1) == 0) && (tex.findByFilename(testfile_2) == 1),
        "retrieve items by filename"
    );

    // ...................................................................... //

    UNITTEST_FINALIZE;
}
