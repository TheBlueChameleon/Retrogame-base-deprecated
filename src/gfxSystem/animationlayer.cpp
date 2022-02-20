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

#ifdef DEBUG
#define CHECK_ANIMATIONSTORE_INDEX(ID) { \
        if ( (ID >= this->animationStore.size()) ) { \
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
// static members

    static constexpr auto              INVALID_TAG         = "<-*-invalid-*->";
    static constexpr auto              VOID_REPRESENTATION = "#";
    static constexpr auto              ANGLE_SEPARATOR     = "@";
    static constexpr AnimationLayer::ElementDescriptor INVALID_GRIDELEMENT = {-1, -1};
    static constexpr AnimationLayer::ElementDescriptor VOID_GRIDELEMENT    = {-1,  0};
    static constexpr AnimationLayer::Element           INVALID_ELEMENT     = {-1, {0,0,0}};

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

    const std::vector<AnimationLayer::Element> AnimationLayer::getElements() const
    {
        return elements;
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
        CHECK_ANIMATIONSTORE_INDEX(element.first);
        elements.push_back(element);
    }

    void AnimationLayer::replaceElement(const int index, const Element& element)
    {
        CHECK_ELEMENT_INDEX(index);
        CHECK_ANIMATIONSTORE_INDEX(element.first);
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
        auto root         = doc .child("project");
        auto nodePalette  = root.child("palette");
        auto nodeElements = root.child("elements");

        reset();

        if (!nodeElements)
        {
            throw InvalidFileError(THROWTEXT("  Could not find tag 'elements' in file '"s + filename + "'"));
        }

        std::vector<int> paletteIndexToStoreIndex;
        if (nodePalette)
        {
            auto paletteFiles = getPaletteEntries(nodePalette);

            for (const auto& file : paletteFiles)
            {
                paletteIndexToStoreIndex.push_back( animationStore.addAnimation(file) );
            }
        }

        for (auto& subNode : nodeElements)
        {
            if ( std::strcmp(subNode.name(), "grid") == 0 )
            {
                auto gridElements = parseGridNode(subNode, paletteIndexToStoreIndex,filename);
                elements.insert(
                    elements.end(),
                    gridElements.begin(), gridElements.end()
                );
            }
            else if ( std::strcmp(subNode.name(), "element") == 0 )
            {
                auto data = parseElementTag(subNode, paletteIndexToStoreIndex);
                if (data == INVALID_ELEMENT)
                {
                    std::cerr << "Warning: element tag in Scene definition " << filename << std::endl;
                    continue;
                }
                elements.push_back( data );
            }
            else
            {
                std::cerr << "Warning: invalid tag '" << subNode.name() << "' in Scene definition " << filename << std::endl;
            }
        }
    }

    std::vector<std::string> AnimationLayer::getPaletteEntries(pugi::xml_node parent) const
    {
        decltype(getPaletteEntries(parent)) reVal;

        for (auto node : parent)
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

    std::vector<AnimationLayer::Element> AnimationLayer::parseGridNode(
        pugi::xml_node node,
        const std::vector<int>& palette,
        const std::string& filename
    ) const
    {
        std::vector<AnimationLayer::Element> reVal;

        const auto spacingX = node.attribute("spacing_x").as_int();
        const auto spacingY = node.attribute("spacing_y").as_int();

        // *INDENT-OFF*
        bool abortParse = false;

        if (!spacingX) {abortParse = true;}
        if (!spacingY) {abortParse = true;}
        if (abortParse) {
            std::cerr << "Warning: grid without spacing in file " << filename << " (ignored)" << std::endl;
            return reVal;
        }

        if (!node.text())                   {abortParse = true;}
        if (abortParse) {
            std::cerr << "Warning: empty grid in file " << filename << " (ignored)" << std::endl;
            return reVal;
        }
        // *INDENT-ON*

        const auto offsetX  = node.attribute("offset_x") .as_int();
        const auto offsetY  = node.attribute("offset_y") .as_int();

        int x = offsetX, y = offsetY;

        const auto rows = split_nonowning(node.text().get(), "\n");
        for (const auto& row : rows)
        {
            if (!row.length())
            {
                continue;
            }

            for (auto& cell : split_nonowning(trim_nonowning(row), " "))
            {
                const auto trimmedCell   = trim_nonowning(cell);
                const auto IdAndRotation = parseGridElement(trimmedCell, palette);

                if (IdAndRotation == INVALID_GRIDELEMENT)
                {
                    std::cerr << "Warning: invalid grid element '" << trimmedCell << "'" << std::endl;
                    continue;
                }

                if (IdAndRotation != VOID_GRIDELEMENT)
                {
                    const Coordinate coord = {x, y, IdAndRotation.second};
                    reVal.emplace_back(IdAndRotation.first, coord);
                }

                x += spacingX;
            }

            x = 0;
            y += spacingY;
        }

        return reVal;
    }

    AnimationLayer::ElementDescriptor AnimationLayer::parseGridElement(
        const std::string_view& elementDescriptor,
        const std::vector<int>& palette
    ) const
    {
        if (elementDescriptor == VOID_REPRESENTATION)
        {
            return VOID_GRIDELEMENT;
        }

        int ID, angle;

        const auto startIterator  = elementDescriptor.begin();
        auto       rotationOffset = elementDescriptor.find(ANGLE_SEPARATOR);

        if (rotationOffset == std::string::npos)
        {
            rotationOffset = elementDescriptor.length();
            angle = 0;
        }
        else
        {
            angle = std::stod(
                        std::string(startIterator + rotationOffset + 1,
                                    elementDescriptor.end())
                    );
        }

        // *INDENT-OFF*
        try {ID = std::stoi( std::string(startIterator, startIterator + rotationOffset) );}
        catch (const std::invalid_argument& e) {return INVALID_GRIDELEMENT;}

        if (ID < 0 || ID >= palette.size()) {return INVALID_GRIDELEMENT;}
        // *INDENT-ON*

        return std::make_pair(palette[ID], angle);
    }

    AnimationLayer::Element AnimationLayer::parseElementTag(pugi::xml_node node, const std::vector<int>& palette) const
    {
        int ID       = node.attribute("id").as_int(-1);
        int x        = node.attribute("offset_x").as_int();
        int y        = node.attribute("offset_y").as_int();
        double angle = node.attribute("rotation").as_int();

        if (ID == -1)
        {
            auto attributeFilename = node.attribute("file");
            if (attributeFilename)
            {
                ID = animationStore.addAnimation(attributeFilename.value());
                return Element(ID, {x, y, angle});
            }
            else
            {
                return INVALID_ELEMENT;
            }
        }

        if (ID < 0 || ID >= palette.size())
        {
            return INVALID_ELEMENT;
        }

        return Element(ID, {x, y, angle});
    }

// ========================================================================== //
// namespace
}
