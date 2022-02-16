// ========================================================================== //
// Depenencies

// STL
#include <stdexcept>

// own
#include "window.hpp"
#include "animationlayer.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)


// ========================================================================== //
// namespace

namespace RetrogameBase
{

// ========================================================================== //
// CTor, DTor

    AnimationLayer::AnimationLayer(Window& window) :
        window(window),
        animaitionStore(window.getAnimationStore())
    {}

// ========================================================================== //
// namespace
}
