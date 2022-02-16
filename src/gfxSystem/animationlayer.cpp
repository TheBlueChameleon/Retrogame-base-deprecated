// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <stdexcept>

#include <cstring>
#include <string>
using namespace std::string_literals;

#include <filesystem>
namespace fs = std::filesystem;

// Pugi
#include "../xmlSystem/pugixml.hpp"

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
        auto root         = doc.child("project");
        auto nodePalette  = root.child("palette");
        auto nodeElements = root.child("elements");

        reset();

        std::vector<int> paletteIndexToStoreIndex;
        if (nodePalette)
        {
            auto paletteFiles = getPaletteEntries(nodePalette);

            for (const auto& file : paletteFiles)
            {
                std::cout << file << std::endl;
                paletteIndexToStoreIndex.push_back( animationStore.addAnimation(file) );
            }
        }

        if (!nodeElements)
        {
            throw std::runtime_error(THROWTEXT("  Could not find tag 'elements' in file '"s + filename + "'"));
        }

        std::cout << "pal:" << std::endl;
        for (const auto& x : paletteIndexToStoreIndex)
        {
            std::cout << "  " << x << std::endl;
        }
    }

    std::vector<std::string> AnimationLayer::getPaletteEntries(pugi::xml_node parent) const
    {
        decltype(getPaletteEntries(parent)) reVal;

        for (auto node = parent.first_child(); node; node = node.next_sibling())
        {
            if (std::strcmp(node.name(), "animation"))
            {
                reVal.push_back(INVALID_TAG);
            }
            else
            {
                auto attributeFile = node.attribute("file");
                if (attributeFile.hash_value())
                {
                    reVal.push_back(attributeFile.value());
                }
                else
                {
                    reVal.push_back(INVALID_TAG);
                }
            }
        }

        return reVal;
    }

// ========================================================================== //
// namespace
}
