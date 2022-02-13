// ========================================================================== //
// Depenencies

// STL
#include <string>
using namespace std::string_literals;

#include <filesystem>
namespace fs = std::filesystem;

// own
#include "texturestore.hpp"
#include "animation.hpp"

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



    Animation::Animation(TextureStore& textureStore) :
        textureStore(textureStore)
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

    const std::vector<size_t>& Animation::getFrames() const
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
        return textureStore.getTexture(currentPhase);
    }

// ========================================================================== //
// setters/modifiers

    void Animation::reset()
    {
        frames.clear();
        currentPhase = 0;
        dimension = {-1, -1};
    }

    void Animation::addFrame(size_t ID)
    {
        if ( (ID >= textureStore.size()) )
        {
            throw std::out_of_range(THROWTEXT("  Invalid TextureStore ID: "s + std::to_string(ID)));
        }

        if (dimension == NODIMENSION)
        {
            dimension = textureStore.getImageDimension(ID);
        }
        else
        {
            if (dimension != textureStore.getImageDimension(ID))
            {
                throw std::runtime_error(THROWTEXT("  Invalid frame size!\n"
                                                   "  Expected: "s +
                                                   DIMENSIONSTRING(dimension) + "\n"
                                                   "  Found: " +
                                                   DIMENSIONSTRING(textureStore.getImageDimension(ID))
                                                  ));
            }
        }

        frames.push_back( ID );
    }

    void Animation::addFrame(const std::string& filename)
    {
        addFrame( textureStore.addFrame(filename) );
    }

    void Animation::advanceFrame()
    {
        ++currentPhase;
        currentPhase -= (currentPhase == frames.size()) * frames.size();
    }
}
