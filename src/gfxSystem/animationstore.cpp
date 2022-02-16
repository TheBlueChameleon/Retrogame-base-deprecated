// ========================================================================== //
// Depenencies

// STL
#include <string>
using namespace std::string_literals;

#include <filesystem>
namespace fs = std::filesystem;

// own
#include "animationstore.hpp"
#include "window.hpp"
#include "texturestore.hpp"

// ========================================================================== //
// local macro

#define THROWTEXT(msg) ("RUNTIME EXCEPTION IN "s + (__PRETTY_FUNCTION__) + "\n"s + msg)

#ifdef DEBUG
#define CHECK_ANIMATION_INDEX(ID) { \
        if ( (ID >= this->size()) ) { \
            throw std::out_of_range(THROWTEXT( "  Invalid AnimationStore ID: "s + std::to_string(ID) )); \
        } \
    }
#else
#define CHECK_GFX_INDEX(ID) {}
#endif

#define CHECK_FILE_EXISTS(filename) {\
        if (!fs::exists(filename)) { \
            throw std::runtime_error(THROWTEXT( \
                                                "  file not found: '"s + filename + "'" \
                                              )); \
        } \
    }

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
// getters

    const Window& AnimationStore::getWindow() const
    {
        return window;
    }

    const TextureStore& AnimationStore::getTextureStore() const
    {
        return textureStore;
    }

    size_t AnimationStore::size() const
    {
        return animations.size();
    }

    Animation& AnimationStore::getAnimation(size_t ID)
    {
        CHECK_ANIMATION_INDEX(ID);
        return animations[ID];

    }

    const std::string& AnimationStore::getFilename(const int ID) const
    {
        CHECK_ANIMATION_INDEX(ID);
        return filenames[ID];
    }

    std::pair<int, int> AnimationStore::getAnimationDimension(const int ID) const
    {
        CHECK_ANIMATION_INDEX(ID);
        return animations[ID].getDimension();
    }

    size_t AnimationStore::findByFilename(const std::string& filename) const
    {
        auto searchIterator = std::find(filenames.begin(),
                                        filenames.end(),
                                        filename);
        if (searchIterator != filenames.end())
        {
            return std::distance(filenames.begin(), searchIterator);
        }
        else
        {
            return NOINDEX;
        }
    }

// ========================================================================== //
// setters/modifiers

    void AnimationStore::reset()
    {
        window.resetStores(Window::ResetStoresDepth::Animations);
    }

    void AnimationStore::reset_private()
    {
        filenames.clear();
        animations.clear();
    }

    size_t AnimationStore::addAnimation(const std::string& filename)
    {
        CHECK_FILE_EXISTS(filename);

        auto index = findByFilename(filename);

        if (index != NOINDEX)
        {
            return index;
        }

        auto& newAnimation = animations.emplace_back(this->window);
        newAnimation.loadXML(filename);

        return animations.size() - 1;
    }

    void AnimationStore::advanceAll()
    {
        for (auto& animation : animations)
        {
            animation.advanceFrame();
        }
    }

// ========================================================================== //
// namespace
}
