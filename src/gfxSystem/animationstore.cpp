// ========================================================================== //
// Depenencies

// STL


// own
#include "animationstore.hpp"
#include "window.hpp"
#include "texturestore.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

// ========================================================================== //
// namespace

namespace RetrogameBase
{
// ========================================================================== //
// CTor, DTor

    AnimationStore::AnimationStore(Window& window) :
        window(window),
        textureStore(window.getTextureStore())
    {}

// ========================================================================== //
// namespace
}
