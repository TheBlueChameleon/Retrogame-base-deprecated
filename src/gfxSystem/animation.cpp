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

// own
#include "window.hpp"
#include "texturestore.hpp"
#include "animation.hpp"
#include "../xmlSystem/xmlwrapper.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

#define CHECK_FILE_EXISTS(filename) { \
        if (!fs::exists(filename)) { \
            throw std::runtime_error(THROWTEXT("  file not found: '"s + filename + "'")); \
        } \
    }

#define DIMENSIONSTRING(dimPair) std::to_string(dimPair.first) + "x" + std::to_string(dimPair.second)

// ========================================================================== //
// namespace

namespace RetrogameBase
{

// ========================================================================== //
// CTor, DTor

    Animation::Animation(Window& window) :
        window(window),
        textureStore(window.getTextureStore())
    {}

    Animation::Animation(Window& window, const std::string& filename) :
        window(window),
        textureStore(window.getTextureStore())
    {
        loadXML(filename);
    }

// ========================================================================== //
// getters

    size_t Animation::size() const
    {
        return frames.size();
    }

    const std::pair<int, int>& Animation::getDimension() const
    {
        return dimension;
    }

    const std::vector<size_t>& Animation::getFrameIDs() const
    {
        return frames;
    }

    size_t Animation::getCurrentPhase() const
    {
        return currentPhase;
    }

    size_t Animation::getCurrentFrameID() const
    {
        return frames[currentPhase];
    }

    SDL_Texture* Animation::getCurrentFrameTexture() const
    {
        return textureStore.getTexture(frames[currentPhase]);
    }

// ========================================================================== //
// setters/modifiers

    void Animation::reset()
    {
        frames.clear();
        currentPhase = 0;
        dimension = NODIMENSION;
    }

    void Animation::advanceFrame()
    {
        ++currentPhase;
        currentPhase -= (currentPhase == frames.size()) * frames.size();
    }

    void Animation::addFrame(size_t ID, size_t repetitions)
    {
        if ( (ID >= textureStore.size()) )
        {
            throw std::out_of_range(THROWTEXT("  Invalid TextureStore ID: "s + std::to_string(ID)));
        }

        if (dimension == NODIMENSION)
        {
            dimension = textureStore.getTextureDimension(ID);
        }
        else
        {
            if (dimension != textureStore.getTextureDimension(ID))
            {
                throw std::runtime_error(THROWTEXT("  Invalid frame dimension!\n"
                                                   "  Expected: "s +
                                                   DIMENSIONSTRING(dimension) + "\n"
                                                   "  Found: " +
                                                   DIMENSIONSTRING(textureStore.getTextureDimension(ID))
                                                  ));
            }
        }

        for (auto i = 0u; i < repetitions; ++i)
        {
            frames.push_back( ID );
        }
    }

    void Animation::addFrame(const std::string& filename, size_t repetitions)
    {
        addFrame( textureStore.addFrame(filename), repetitions );
    }

    void Animation::loadXML(const std::string& filename)
    {
        auto doc = XmlLoad(filename, "animation");
        auto root = doc.child("project");
        auto nodeAnimation = root.child("animation");

        if (!nodeAnimation)
        {
            throw std::runtime_error(THROWTEXT("  Could not find tag 'animation' in file '"s + filename + "'"));
        }

        reset();
        for (auto subNode = nodeAnimation.first_child(); subNode; subNode = subNode.next_sibling())
        {
            const auto data = getFilenameAndRepetitionFromTag(subNode, filename);

            if (data == INVALID_TAG)
            {
                std::cerr << "Warning: invalid tag in Animation Definition " << filename << std::endl;
                continue;
            }
            else
            {
                const auto& [frameFile, repeat] = data;
                addFrame(frameFile, repeat);
            }
        }
    }

    const std::pair<std::string, int> Animation::INVALID_TAG  = {"<-*-invalid-*->", -1};

    std::pair<std::string, int> Animation::getFilenameAndRepetitionFromTag(pugi::xml_node& node, const std::string& filename) const
    {
        if (std::strcmp(node.name(), "frame"))
        {
            return INVALID_TAG;
        }

        std::string frameFile = node.attribute("file").value();
        int         repeat    = node.attribute("repeat").as_int(1);

        return std::make_pair(frameFile, repeat);
    }
}
