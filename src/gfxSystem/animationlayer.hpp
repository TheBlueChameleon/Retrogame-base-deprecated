#ifndef ANIMATIONLAYER_HPP
#define ANIMATIONLAYER_HPP

// ========================================================================== //
// Depenencies

// STL
#include <stdexcept>

#include <vector>
#include <tuple>

// own

// ========================================================================== //
// Class

namespace RetrogameBase
{
    class Window;
    class Animation;
    class AnimationStore;

    class AnimationLayer
    {
        public:
            using Coordinate = std::pair<int, int>;
            using Element    = std::pair<int, Coordinate>;

        private:
            Window& window;
            AnimationStore& animationStore;

            std::vector<Element> elements;

        public:

            // -------------------------------------------------------------- //
            // CTor, DTor
            AnimationLayer(RetrogameBase::Window& window);

            // -------------------------------------------------------------- //
            // getters

            size_t size() const;

            Element    getElement    (const int index) const;
            int        getAnimationID(const int index) const;
            Animation& getAnimation  (const int index) const;
            Coordinate getCoordinate (const int index) const;

            // -------------------------------------------------------------- //
            // setters/modifiers

            void reset();

            void addElement(const Element& element);
            void removeElement(const int index);

            // -------------------------------------------------------------- //
            // display
    };

}
#endif // ANIMATIONLAYER_HPP
