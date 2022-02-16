// ========================================================================== //
// Depenencies

// STL
#include <stdexcept>

#include <string>
using namespace std::string_literals;

#include <filesystem>
namespace fs = std::filesystem;

// own
#include "window.hpp"
#include "animationstore.hpp"
#include "animationlayer.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

#ifdef DEBUG
#define CHECK_ELEMENT_INDEX(ID) { \
        if ( (ID >= this->size()) ) { \
            throw std::out_of_range(THROWTEXT( "  Invalid AnimationStore ID: "s + std::to_string(ID) )); \
        } \
    }
#else
#define CHECK_ELEMENT_INDEX(ID) {}
#endif

// ========================================================================== //
// namespace

namespace RetrogameBase
{

// ========================================================================== //
// CTor, DTor

    AnimationLayer::AnimationLayer(Window& window) :
        window(window),
        animationStore(window.getAnimationStore())
    {}

// ========================================================================== //
// getters

    size_t AnimationLayer::size() const
    {
        return elements.size();
    }

    AnimationLayer::Element AnimationLayer::getElement(const int index) const
    {
        CHECK_ELEMENT_INDEX(index);
        return elements[index];
    }

    int AnimationLayer::getAnimationID(const int index) const
    {
        CHECK_ELEMENT_INDEX(index);
        return elements[index].first;
    }

    Animation& AnimationLayer::getAnimation(const int index) const
    {
        CHECK_ELEMENT_INDEX(index);
        return animationStore.getAnimation( elements[index].first );
    }

    AnimationLayer::Coordinate AnimationLayer::getCoordinate(const int index) const
    {
        CHECK_ELEMENT_INDEX(index);
        return elements[index].second;
    }

// ========================================================================== //
// setters/modifiers

    void AnimationLayer::reset()
    {
        elements.clear();
    }

    void AnimationLayer::addElement(const Element& element)
    {
        elements.push_back(element);
    }

    void AnimationLayer::replaceElement(const int index, const Element& element)
    {
        CHECK_ELEMENT_INDEX(index);
        elements[index] = element;
    }

    void AnimationLayer::replaceElement(const int index, const int animationID)
    {
        CHECK_ELEMENT_INDEX(index);
        elements[index].first = animationID;
    }

    void AnimationLayer::removeElement(const int index)
    {
        CHECK_ELEMENT_INDEX(index);
        elements.erase( elements.begin() + index );
    }

    void AnimationLayer::moveElement(const int index, const Coordinate& coordinate)
    {
        CHECK_ELEMENT_INDEX(index);
        elements[index].second = coordinate;
    }

    void AnimationLayer::loadXML(const std::string& filename)
    {
        auto doc = XmlLoad(filename, "animationlayer");
        auto root = doc.child("project");
        auto nodePalette = root.child("palette");
        auto nodeAnimationLayer = root.child("animationlayer");

        if (nodePalette) {}

        if (!nodeAnimationLayer)
        {
            throw std::runtime_error(THROWTEXT("  Could not find tag 'animationlayer' in file '"s + filename + "'"));
        }


    }

// ========================================================================== //
// namespace
}
