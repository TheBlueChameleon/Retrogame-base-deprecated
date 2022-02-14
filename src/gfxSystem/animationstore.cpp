// ========================================================================== //
// Depenencies

// STL


// own
#include "animationstore.hpp"
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

    AnimationStore::AnimationStore(TextureStore& textureStore) :
        textureStore(textureStore)
    {}

// ========================================================================== //
// namespace
}
