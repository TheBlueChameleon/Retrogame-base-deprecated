// ========================================================================== //
// Depenencies

// STL
#include <iostream>
#include <stdexcept>

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
        const auto animationTags = XmlExtractSimpleGroup(nodeAnimation);

        for (auto& tag : animationTags)
        {
            const auto data = getFilenameAndRepetitionFromTag(tag, filename);

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

    const std::pair<std::string, int> Animation::INVALID_TAG  = {"<--*invalid*-->", -1};

    std::pair<std::string, int> Animation::getFilenameAndRepetitionFromTag(XmlSimpleTag tag, const std::string& filename)
    {
        if (!(tag.first == "frame"))
        {
            return INVALID_TAG;
        }

        std::string frameFile;
        int         repeat = 1;

        bool hasFilename = false;
        bool hasRepeat   = false;

        for (auto& property : tag.second)
        {
            if (property.first == "file")
            {
                if (hasFilename)
                {
                    std::cerr << "Warning: duplicate definition of filename\n"
                              << "  Animation Definition    : " << filename << "\n"
                              << "  Previous Frame Reference: " << frameFile << "\n"
                              << "  New Frame Reference     : " << property.second << " (ignored)"
                              << std::endl;
                    continue;
                }
                hasFilename = true;
                frameFile = property.second;

            }
            else if (property.first == "repeat")
            {
                if (hasRepeat)
                {
                    std::cerr << "Warning: duplicate definition of frame repetition\n"
                              << "  Animation Definition    : " << filename << "\n"
                              << "  Previous Frame Reference: " << repeat << "\n"
                              << "  New Frame Reference     : " << property.second << " (ignored)"
                              << std::endl;
                    continue;
                }
                try
                {
                    repeat = std::stoi(property.second);
                }
                catch (const std::invalid_argument& e)
                {
                    std::cerr << "Warning: invalid definition of frame repetition\n"
                              << "  Animation Definition: " << filename << "\n"
                              << "  Repetition Value    : " << property.second << " (ignored)"
                              << std::endl;
                    continue;
                }
                hasRepeat = true;
            }
        }

        if (!hasFilename)
        {
            return INVALID_TAG;
        }

        return std::make_pair(frameFile, repeat);
    }
}
