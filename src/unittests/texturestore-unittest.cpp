// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <stdexcept>

#include <filesystem>
namespace fs = std::filesystem;

#include <utility>

// own
#include "../globals.hpp"
#include "../gfxSystem/window.hpp"
#include "../gfxSystem/texturestore.hpp"
#include "texturestore-unittest.hpp"

using namespace RetrogameBase;

// ========================================================================== //
// tests


bool unittest_TextureStore_addReset()
{
    std::cout << "TESTING TEXTURESTORE ADD AND RESET METHODS" << std::endl;

    bool result = true;
    bool last = true;
    size_t ID;

    constexpr auto testfile_1 = "../gfx/sea01.png";
    constexpr auto testfile_2 = "../gfx/sea02.png";

    constexpr auto dimensions_1 = std::make_pair(50, 50);
    constexpr auto dimensions_2 = std::make_pair(50, 50);

    Window win("test window");
    TextureStore& tex = win.getTextureStore();

    // ...................................................................... //

    last = tex.size() == 0;
    if (last)
    {
        std::cout << "  succeeded to set up empty gfx store by default" << std::endl;
    }
    else
    {
        std::cout << "  failed to set up empty gfx store by default" << std::endl;
    }
    result &= last;

    // ...................................................................... //

    tex.reset();
    last = tex.size() == 0;
    if (last)
    {
        std::cout << "  succeeded to reset empty gfx store by default" << std::endl;
    }
    else
    {
        std::cout << "  failed to reset empty gfx store by default" << std::endl;
    }
    result &= last;

    // ...................................................................... //

    last = fs::exists(testfile_1) && fs::exists(testfile_2);
    if (last)
    {
        std::cout << "  found test files on disk" << std::endl;
    }
    else
    {
        std::cout << "  either or both of test files '" << testfile_1 << "' and "
                  << "'" << testfile_2 << "' were not found." << std::endl;
        return false;
    }

    // ...................................................................... //

    try
    {
        ID = tex.addFrame(testfile_1);
        last = true;
    }
    catch (const std::runtime_error& e)
    {
        last = false;
    }
    if (last)
    {
        std::cout << "  succeeded to load existing file" << std::endl;
    }
    else
    {
        std::cout << "  failed to load existing file" << std::endl;
    }
    result &= last;

    // ...................................................................... //

    last = ID == 0;
    if (last)
    {
        std::cout << "  succeeded to return correct index for first texture" << std::endl;
    }
    else
    {
        std::cout << "  failed to return correct index for first texture" << std::endl;
    }
    result &= last;

    // ...................................................................... //

    try
    {
        ID = tex.addFrame(testfile_1);
        last = ID == 0;
    }
    catch (const std::runtime_error& e)
    {
        last = false;
    }

    if (last)
    {
        std::cout << "  succeeded in preventing loading same file twice" << std::endl;
    }
    else
    {
        std::cout << "  failed in preventing loading same file twice" << std::endl;
    }
    result &= last;

    // ...................................................................... //

    try
    {
        ID = tex.addFrame(testfile_2);
        last = ID == 1;
    }
    catch (const std::runtime_error& e)
    {
        last = false;
    }

    if (last)
    {
        std::cout << "  succeeded in loading a second file" << std::endl;
    }
    else
    {
        std::cout << "  failed in loading a second file" << std::endl;
    }
    result &= last;

    // ...................................................................... //

    last = tex.size() == 2;
    if (last)
    {
        std::cout << "  succeeded to record size correctly after adding textures" << std::endl;
    }
    else
    {
        std::cout << "  failed to record size correctly after adding textures" << std::endl;
    }
    result &= last;

    // ...................................................................... //

    last = (tex.getTexture(0) != nullptr) && (tex.getTexture(1) != nullptr);
    if (last)
    {
        std::cout << "  succeeded in obtaining valid texture ptr" << std::endl;
    }
    else
    {
        std::cout << "  failed in obtaining valid texture ptr" << std::endl;
    }
    result &= last;

    // ...................................................................... //

    last = (tex.getImageDimensions(0) == dimensions_1) && (tex.getImageDimensions(1) == dimensions_2);
    if (last)
    {
        std::cout << "  succeeded in retrieving correct dimensions" << std::endl;
    }
    else
    {
        std::cout << "  failed in retrieving correct dimensions" << std::endl;
    }
    result &= last;

    // ...................................................................... //

    last = (tex.getFilename(0) == testfile_1) && (tex.getFilename(1) == testfile_2);
    if (last)
    {
        std::cout << "  succeeded in recording filenames" << std::endl;
    }
    else
    {
        std::cout << "  failed in recording filenames" << std::endl;
    }
    result &= last;

    // ...................................................................... //

    last = (tex.findByFilename(testfile_1) == 0) && (tex.findByFilename(testfile_2) == 1);
    if (last)
    {
        std::cout << "  succeeded in retrieving items by filename" << std::endl;
    }
    else
    {
        std::cout << "  failed in retrieving items by filename" << std::endl;
    }
    result &= last;

    // ...................................................................... //

    return result;
}
