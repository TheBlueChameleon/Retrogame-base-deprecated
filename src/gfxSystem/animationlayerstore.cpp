// ========================================================================== //
// Depenencies

// STL
#include <exception>

#include <functional>

#include <string>
using namespace std::string_literals;

// own
#include "window.hpp"
#include "animationlayerstore.hpp"
#include "animationlayer.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

#ifdef DEBUG
#define CHECK_ANIMATIONLAYER_INDEX(ID) { \
        if ( (ID >= this->animationLayers.size()) ) { \
            throw std::out_of_range(THROWTEXT( "  Invalid AnimationLayer ID: "s + std::to_string(ID) )); \
        } \
    }
#else
#define CHECK_ANIMATIONLAYER_INDEX(ID) {}
#endif

// ========================================================================== //
// namespace

namespace RetrogameBase
{

// ========================================================================== //
// CTor, DTor

    RetrogameBase::AnimationLayerStore::AnimationLayerStore(Window& window) :
        window(window)
    {}

// ========================================================================== //
// Getters

    size_t AnimationLayerStore::size() const
    {
        return animationLayers.size();
    }

    AnimationLayer& AnimationLayerStore::getLayer(size_t ID)
    {
        CHECK_ANIMATIONLAYER_INDEX(ID);
        return animationLayers[ID];
    }

// ========================================================================== //
// Setters, Modifiers

    void AnimationLayerStore::reset()
    {
        window.resetStores(Window::ResetStoresDepth::Layers);
    }

    void AnimationLayerStore::reset_private()
    {
        animationLayers.clear();
    }

    std::pair<size_t, AnimationLayer&> AnimationLayerStore::addLayer()
    {
        auto& layer = animationLayers.emplace_back(window);
        return std::make_pair(animationLayers.size() - 1, std::reference_wrapper(layer));
    }

    std::pair<size_t, AnimationLayer&> AnimationLayerStore::addLayer(const std::string& filename)
    {
        auto& layer = animationLayers.emplace_back(window);
        size_t ID   = animationLayers.size() - 1;

        layer.loadXML(filename);

        return std::make_pair(ID, std::reference_wrapper(layer));
    }

}
