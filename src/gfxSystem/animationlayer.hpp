#ifndef ANIMATIONLAYER_HPP
#define ANIMATIONLAYER_HPP

// ========================================================================== //
// Depenencies

// STL
#include <stdexcept>

#include <vector>
#include <tuple>

// Pugi
#include "../xmlSystem/pugixml.hpp"

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
            using Coordinate = std::tuple<int, int, double>;        // x, y, angle
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

            void addElement    (                 const Element& element);
            void replaceElement(const int index, const Element& element);
            void replaceElement(const int index, const int      animationID);
            void removeElement (const int index);
            void moveElement   (const int index, const Coordinate& coordinate);

            void loadXML (const std::string& filename);
        private:
            static constexpr auto INVALID_TAG = "<-*-invalid-*->";
            std::vector<std::string> getPaletteEntries(pugi::xml_node node) const;
        public:

            // -------------------------------------------------------------- //
            // display
    };

}
#endif // ANIMATIONLAYER_HPP
